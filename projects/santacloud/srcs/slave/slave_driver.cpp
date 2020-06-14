#include <sys/types.h>  // socket
#include <sys/socket.h> // socket
#include <string.h> // strerror
#include <arpa/inet.h> // htons
#include <unistd.h> // read
#include <sstream> // ostringstream
#include <strings.h> //bzero

#include "tools.hpp" 
#include "exceptions.hpp" // DriverError
#include "lockguard.hpp" // LockGuard
#include "config_data.hpp" // ConfigDataSlave
#include "slave_driver.hpp" 

namespace ilrd
{

SlaveDriver::SlaveDriver()
	: MAX_MSG_SIZE(Handleton<ConfigDataSlave>::get_instance()->m_max_req_size +
	 MAX_HEADER_SIZE), m_logger(get_logger())
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		throw DriverError("socket open", strerror(errno));
	}
	
	// configuration data
	ConfigDataSlave* conf_data = Handleton<ConfigDataSlave>::get_instance();
	
	struct sockaddr_in master_addr;
	memset(&master_addr, 0, sizeof(master_addr));
	
	master_addr.sin_family = AF_INET;
	master_addr.sin_port = htons(conf_data->m_port_num);
	inet_aton(conf_data->m_master_addr.c_str(), &(master_addr.sin_addr));
	
	if(connect(sockfd, (struct sockaddr *)&master_addr, sizeof(master_addr)) < 0)
	{
		throw DriverError("socket connect", strerror(errno));
	}
	
	set_request_fd(sockfd);
}


void SlaveDriver::ping_master()
{
	int fd = get_request_fd();
	
	std::string ping_msg("Hi Master I am your slave");
	
	ProxyDriver::write_all(fd, ping_msg.c_str(), ping_msg.size());
	
	LOG(Logger::INFO, "sent master a message", __LINE__);
}

std::unique_ptr<DriverData> SlaveDriver::receive_request()
{	
	int fd = get_request_fd();
	
	std::vector<char> read_buffer(MAX_MSG_SIZE);
	
	int read_bytes = read(fd, read_buffer.data(), MAX_MSG_SIZE); 
	
	if(-1 == read_bytes)
	{
		LOG(Logger::ERROR, "error read request", __LINE__);
		throw DriverError("receive_request", strerror(errno));
	}
	
	unsigned long int offset;
	int length;
	int req_id;
	int req_type;
	
	// parsing header
	sscanf(read_buffer.data(), "%lu;%d;%d;%d;", &offset, &length, &req_id, &req_type);
	
	DriverData::action_type type = static_cast<DriverData::action_type>(req_type);
	
	std::unique_ptr<DriverData> request(new DriverData(type, offset, length, 
									    req_id));
	
	std::vector<char>& req_buff = request->access_buffer();
	if(type == DriverData::WRITE)
	{
		// positioning on end of header
		char* runner = read_buffer.data();
		while('\0' != *runner)
		{
			++runner;
		}
		++runner;

		std::copy(runner, runner + length, req_buff.data());
	}
	
	std::ostringstream log_msg;
	log_msg << "request from master. type: " << type <<  " offset: " << offset 
	<< " "  << " length: " << length << " id: " << req_id;
	
	LOG(Logger::INFO, log_msg.str(), __LINE__);
	
	return request;
}

void SlaveDriver::send_reply(std::unique_ptr<DriverData> data)
{
	int fd = get_request_fd();
	
	int length = data->get_length();
	int req_id = data->get_req_id();
	int req_type = data->get_type();
	std::vector<char>& req_buffer = data->access_buffer();
	
	std::ostringstream log_msg;
	log_msg << "replying master. type: " << req_type  
	<< " "  << "length: " << length << " id: " << req_id;
	LOG(Logger::INFO, log_msg.str(), __LINE__);
	
	// format buffer to write
	std::vector<char> buffer_to_write(MAX_MSG_SIZE);
	
	int delim = sprintf(buffer_to_write.data(), "%d;%d;%d;%c", length, req_id, 
					req_type, 0);
	if(DriverData::READ == req_type)
	{
		std::copy(req_buffer.data(), req_buffer.data() + length, 
					buffer_to_write.data() + delim);

	}
	
	LockGuard<std::mutex> lock(m_mutex);
	
	// length of write depends on type of request
	int bytes_written = write(fd, buffer_to_write.data(), MAX_HEADER_SIZE + 
					(DriverData::READ == req_type) * length);
	
	if(-1 == bytes_written)
	{
		LOG(Logger::ERROR, "error writing reply", __LINE__);
		throw DriverError("send_reply", strerror(errno));
	}
	
}

void SlaveDriver::disconnect()
{
}

void SlaveDriver::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, RED "SlaveDriver" RESET, msg, line);
}

/****************************************************

	Documentation
	
***************************************************
	
	This module is responsible for communicating with 
	master, receiving requests and sending replies.
	
	* Construction
      opens a UDP socket, and connects it to the address of master.
	  sends master a message.
	
	* std::unique_ptr<DriverData> SlaveDriver::receive_request()
	  - reads request into a local buffer
	  - parsing the request information
	  - creates a DriverData. if write requests, copy the data.
	
	* void SlaveDriver::send_reply(std::unique_ptr<DriverData> data)
	  - creates a local buffer. formatting the reply to that buffer
	    and writing it to the socket.

*********************************************************/

} // ilrd
