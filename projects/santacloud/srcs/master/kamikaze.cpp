#include <string.h> // strerror
#include <sstream> // ostringstream
#include <sys/types.h>    // socket
#include <sys/socket.h>   // socket
#include <strings.h> // bzero
#include <arpa/inet.h> // htons
#include <netinet/in.h>  // bind
#include <unistd.h> // read
#include <sstream> // ostringstream
#include <chrono> // now

#include "handleton.hpp" // Handleton
#include "nbd_data.hpp" // NBDData
#include "logger.hpp" // Logger
#include "exceptions.hpp" // DiskError

#include "tools.hpp"
#include "kamikaze.hpp"

namespace ilrd
{

// priority_queue arranges the queue so the 'biggest' element is on top,
// i want the smallest, i.e., the earliest, on top.
bool operator<(const RequestTime& lhs, const RequestTime& rhs)
{
	return lhs.m_time_stamp > rhs.m_time_stamp;
}

LiveRequest::LiveRequest(std::shared_ptr<DriverData> data, bool is_backup)
	: m_data(data), m_is_backup(is_backup)
{
}

RequestTime::RequestTime(int req_id, 
		std::chrono::time_point<std::chrono::high_resolution_clock> time_stamp)
	: m_req_id(req_id), m_time_stamp(time_stamp)
{	
}

Kamikaze::Kamikaze()
	: m_conf_data(Handleton<ConfigDataMaster>::get_instance()),
	MAX_MSG_SIZE(m_conf_data->m_max_req_size + MAX_HEADER_SIZE),
	m_mutexes(m_conf_data->m_ports.size()), 
	m_track_thread(&Kamikaze::tracker, this), m_should_run(true), 
	m_logger(get_logger())
{	
	connect_to_slaves();
}

void Kamikaze::connect_to_slaves()
{
	int num_slaves = m_conf_data->m_ports.size();
	for(int i = 0; i < num_slaves; ++i)
	{
		int slave_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == slave_sock)
		{
			LOG(Logger::ERROR, "open socket failed", __LINE__);
			throw DiskError(std::string("open socket failed: ") +
			std::string(strerror(errno)));
		}
		
		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));
	
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		int port = m_conf_data->m_ports[i];
		addr.sin_port = htons(port);
	
		if(-1 == bind(slave_sock, (struct sockaddr*)&addr, sizeof(addr)))
		{
			LOG(Logger::ERROR, "bind failed", __LINE__);
			throw DiskError(std::string("bind failed: ") + strerror(errno));
		}
		
		std::ostringstream msg_wait;
		msg_wait << "waiting for a slave to connect to: " << port;
		LOG(Logger::INFO, msg_wait.str(), __LINE__); 
		pong_slave(slave_sock);
		
		std::ostringstream msg_confirm;
		
		msg_confirm << "slave connected to port: " << port;
		LOG(Logger::INFO, msg_confirm.str(), __LINE__);
		m_sockets.push_back(slave_sock);

	}
	
	LOG(Logger::INFO, "all slaves connected!", __LINE__);
}

void Kamikaze::pong_slave(int socket)
{	
	std::string verify("Hi Master I am your slave");
	const std::size_t msg_len = verify.size();
	std::string msg;
	msg.resize(msg_len);
	
	struct sockaddr_in slave_addr;
	bzero(&slave_addr, sizeof(slave_addr));
	socklen_t len = sizeof(slave_addr);
	
	recvfrom(socket, &msg[0], msg_len, MSG_WAITALL, 
			 (struct sockaddr*)&slave_addr, &len);
	if(verify == msg)
	{
		if(-1 == connect(socket, (struct sockaddr*)&slave_addr, 
						 sizeof(slave_addr)))
		{
			throw DiskError(std::string("connect failed: ") + 
							std::string(strerror(errno)));
		}
	}
	else
	{
		LOG(Logger::WARNING, "received wrong message, try again", __LINE__);
		pong_slave(socket);
	}
}

Kamikaze::~Kamikaze()
{
	LOG(Logger::DEBUG, "Dtor", __LINE__);
	for(std::size_t i = 0; i < m_conf_data->m_ports.size(); ++i)
	{
		close(m_conf_data->m_ports[i]);
	}
	m_should_run = false;
	m_track_thread.join();
	LOG(Logger::DEBUG, "thread joined", __LINE__);
}

void Kamikaze::send_request(std::unique_ptr<DriverData> data)
{
	std::shared_ptr<DriverData> sp(std::move(data));
	
	send_request_imp(sp, false);
	if(sp->get_type() == DriverData::WRITE)
	{
		send_request_imp(sp, true);
	}
}

void Kamikaze::send_request_imp(std::shared_ptr<DriverData> data, bool is_backup)
{
	std::size_t slave_size = m_conf_data->m_slave_mem_size;
	int num_slaves = m_conf_data->m_ports.size();
	int max_req = m_conf_data->m_max_req_size;
	
	// if backup, we want the next slave
	int slave_num = (data->get_offset() / slave_size + is_backup) % num_slaves;
	
	// if backup, offset is + slave_mem_size + max_req. see documentation
	std::size_t offset = (data->get_offset() % slave_size) + 
										(is_backup * (slave_size + max_req));
	
	int length = data->get_length();
	std::vector<char>& buffer = data->access_buffer();
	DriverData::action_type type = data->get_type();
	
	// pushing to map
	static int req_id = 0; // numbering requests ids
	m_map_mutex.lock();
	int curr_id = req_id++;
	m_live_reqs.insert({curr_id, LiveRequest(data, is_backup)});
	m_map_mutex.unlock();
	
	m_time_queue.push(RequestTime(curr_id, std::chrono::high_resolution_clock::now()));
	
	
	
	// format buffer to write to socket
	std::vector<char> buffer_to_write(MAX_MSG_SIZE);
	
	int delim = sprintf(buffer_to_write.data(), "%lu;%d;%d;%d;%c", offset, length, 
						curr_id, type, 0);
	
	if(DriverData::WRITE == type)
	{
		std::copy(buffer.data(), buffer.data() + length, buffer_to_write.data() + delim);
	}
	// write to slave
	
	std::lock_guard<std::mutex> lock(m_mutexes[slave_num]);
	
	int bytes_written = write(m_sockets[slave_num], buffer_to_write.data(), 
	MAX_HEADER_SIZE + length * (DriverData::WRITE == type));
	
	std::string type_str = type == DriverData::READ ? "read" : is_backup ? 
	"write backup" : "write";
	std::ostringstream log_msg;
	log_msg << "requesting slave num: " << slave_num << " type: "  << type_str <<  
	" offset: " << offset << " length: " << length << " id: " << curr_id <<
	" bytes written: " <<  bytes_written;
	LOG(Logger::INFO, log_msg.str(), __LINE__);
	
	if(-1 == bytes_written) 
	{
		std::ostringstream log_msg;
		log_msg << "error writing to slave id: " << curr_id;
		LOG(Logger::ERROR, log_msg.str(), __LINE__);
		throw DiskError(std::string("error writing to slave: ") + 
						strerror(errno));
	}
}

std::unique_ptr<DriverData> Kamikaze::receive_reply(int fd)
{
	std::vector<char> read_buffer(MAX_MSG_SIZE);
	
	int read_bytes = read(fd, read_buffer.data(), MAX_MSG_SIZE);
	
	if(-1 == read_bytes)
	{
		LOG(Logger::ERROR, "had trouble reading reply", __LINE__);
		throw DiskError(std::string("read reply failed: ") + 
						std::string(strerror(errno)));
	}
	
	int length;
	int req_id;
	int req_type;
	char trash;
	sscanf(read_buffer.data(), "%d;%d;%d;%c", &length, &req_id, &req_type, &trash);
	
	std::ostringstream log_msg;
	log_msg << "received reply: id: " << req_id;
	LOG(Logger::INFO, log_msg.str(), __LINE__);
	
	std::unique_ptr<DriverData> reply = nullptr;
	LiveRequest request;
	
	try
	{
		std::lock_guard<std::mutex> lock(m_map_mutex);
		request = m_live_reqs.at(req_id);
		m_live_reqs.erase(req_id);
	}
	catch(std::out_of_range& ex)
	{	
		std::ostringstream log_msg;
		log_msg << "request " << req_id << " is not in map";
		LOG(Logger::WARNING, log_msg.str(), __LINE__);
		
		return reply;
	}
	
	NBDData* orig_request = static_cast<NBDData*>(request.m_data.get());
	
	if(!request.m_is_backup)
	{
		reply.reset(new NBDData(orig_request->get_type(), 
					orig_request->get_handle(), 0, length));
	}
	if(req_type == DriverData::READ)
	{
		std::vector<char>& buffer = reply->access_buffer();
		// positioning on end of header
		char *curr = read_buffer.data();
		
		while(*curr != '\0')
		{
			++curr;
		}
		++curr;
		std::copy(curr, curr + length, buffer.data());
	}

	return reply;
}

void Kamikaze::tracker()
{
	// SET POLICY IN SOME WAY
	const std::chrono::milliseconds policy(m_conf_data->m_req_timeout);
	while(m_should_run)
	{
		bool was_ack = false;
		
		RequestTime oldest_req;
		if(!m_time_queue.pop(oldest_req, std::chrono::seconds(3))) // returned after timeout
		{
			continue;
		} 
		std::chrono::time_point<std::chrono::high_resolution_clock> now =
		std::chrono::high_resolution_clock::now();
		
		// time to sleep = policy - (now - req)
		std::chrono::duration<double, std::ratio<1, 1000> > diff = now -
		oldest_req.m_time_stamp;
		std::this_thread::sleep_for(policy - diff);
		
		std::ostringstream log_msg;
		log_msg << "request " << oldest_req.m_req_id;
		LiveRequest request_from_map;
		
		m_map_mutex.lock();
		try
		{
			request_from_map = m_live_reqs.at(oldest_req.m_req_id);
			log_msg << " was NOT Ack, sending again...";
			LOG(Logger::DEBUG, log_msg.str(), __LINE__);
		
			m_live_reqs.erase(oldest_req.m_req_id);
		}
		catch(std::out_of_range& ex)
		{	
			was_ack = true;
			log_msg << " was Ack";
			LOG(Logger::DEBUG, log_msg.str(), __LINE__);
		}
		m_map_mutex.unlock();
		
		if(!was_ack)
		{
			send_request_imp(request_from_map.m_data, request_from_map.m_is_backup);	
		}
	}
}

int Kamikaze::get_socket_fd(int index)
{
	return m_sockets[index];
}

void Kamikaze::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, std::string(GREEN "Kamikaze" RESET), msg, line);
}

} // ilrd


/****************************************

	Documentation
	
*****************************************

* Construction - 
	upon construction, the Kamikaze module opens sockets and connects to slaves.
	this is done by running a loop that iterates all the ports listed in the 
	configuration file. after binding, it waits for a message from the corresponding
	slave, and connects the socket to the slave's address, so preceding writes 
	to the socket will reach this slave.
	
	m_track_thread is initialized to execute the tracker function.

* send_request(std::unique_ptr<DriverData> data)
	this function converts the data to shared_ptr<DriverData>,
	and calls send_request_imp with this shared_ptr and a boolean that indicates
	whether this is backup or not. upon receiving write requests, it will call
	send_request_imp twice.
	
* send_request_imp(std::shared_ptr<DriverData> data, bool is_backup)
	
	this function is invoked by the SendRequestTask.
	
	this function in steps:
	- claulates the correct slave to send the request to. this is done
	  by dividing the offset in the number of slaves, and adding 1 if the 
	  request is to be sent to backup.
	
	- calculates the offset withing the slave. this is done by calculating
	  the remainder of the above division, AND adding the maximux of a request
	  size. example for explanation:
	  	 let's say our slave is 10000, storage + backup. it can receive a request
	  	 to write in offset 4900, length of max_request (let's say 4096). so we
	  	 have to leave a safety buffer of max_request, and start the backup 
	  	 only after that buffer.
	
	* this class uses to help structs:
		- LiveRequest - holds std::shared_ptr<DriverData> and bool is_backup
		- RequestTime - holds request id and timestamp in milliseconds.
		
	- insert to the map (m_live_requests) the id of the request and an instance
	  of LiveRequest.
	
	- insert to the priority_queue (m_time_queue) an instance of RequestTime.
	  
	  these two steps are for making sure we receive replys, and resend requests
	  if we don't receive them.
	
	- create a buffer to write, format it with header, and, if write request, 
	  the data to be written.
	
	- send the buffer to the slave.
	
* std::unique_ptr<DriverData> receive_reply
	
	this function is invoked by the SlaveGate::read(), which consequentially
	create a SendReplyTask.
	
	steps:
	- create a local buffer, and read the reply from slave to it.
	- parse the header of the request.
	- look for that request in the map (m_live_requests), retreive the corresponding
	  std::shared_ptr<DriverData> (that contains the handle to be sent back to NBD).
	  Note: there's a possibility that the tracker erased this LiveRequest from the map,
	  in that case the function returns a nullptr, and SlaveGate::read() will create
	  a DoNothingTask.
	  
	- if found, erase from the map, and copy to a new std::unique_ptr<DriverData>
	  (if it's a backup request, return a null ptr).
	  if it's a read request, copy the data in the local buffer into that new
	  DriverData.

* tracker()
	
	this function is run by a seperate thread. it's function is to keep track
	of request timestamps, and if it notifies that a certain request didn't
	receive a reply in time (set in configuration), it sends this request again.
	
	steps:
	- while should_run:
	   - pop the priority_queue (blocked if empty)
	   - calculate the time left until timeout, and sleep this period of time.
	   - check if oldest request (top of pq) is steel in map:
	   		- yes: reply didn't arrive: 
	   			- erase from map
	   			- send again.
	   		- no: reply arrived:
	   			- do nothing



**************************************************************************/

