/*************************************
	NBDDriverProxy implementation
	
***************************************/

#include <iostream> // std::cout
#include <sys/types.h>  // socketpair
#include <sys/socket.h> // socketpair
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <linux/nbd.h> // nbd macros
#include <sys/ioctl.h> // ioctl
#include <signal.h> // sigaction
#include <unistd.h> // write
#include <cstring> // strerror
#include <netinet/in.h> // ntohl
#include <assert.h> // assert
#include <sstream> // ostringstream

#include "lockguard.hpp" // LockGuard
#include "tools.hpp"
#include "nbd_driver.hpp"

namespace ilrd
{

Logger* get_logger(); // in libglob.so

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) 
{
	return a;
}
#else
u_int64_t ntohll(u_int64_t a) 
{
	u_int32_t lo = a & 0xffffffff;
	u_int32_t hi = a >> 32U;
	lo = ntohl(lo);
	hi = ntohl(hi);
	return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

// static member functions

template <typename T, typename... Types>
void NBDDriverProxy::call_ioctl(T var1, Types... var2)
{
	int status;
	status = ioctl(var2...);
	if(-1 == status)
	{	
		throw(DriverError(var1, strerror(errno)));
	}
}

NBDDriverProxy::NBDDriverProxy(const std::string& dev_file, size_t storage_size)
	: m_is_disconnected(false), m_logger(get_logger())
{
	
	setup(dev_file);
	call_ioctl("NBD_SET_SIZE", m_nbd_fd.get(), NBD_SET_SIZE, storage_size);
	
	// open thread
	m_nbd_thread = std::thread(&NBDDriverProxy::call_do_it, this);
}

NBDDriverProxy::NBDDriverProxy(const std::string& dev_file, 
                   				size_t block_size, size_t num_blocks)
	: m_is_disconnected(false)
{
	setup(dev_file);
	call_ioctl("NBD_SET_BLKSIZE",m_nbd_fd.get(), NBD_SET_BLKSIZE, block_size);
	call_ioctl("NBD_SET_SIZE_BLOCKS", m_nbd_fd.get(), NBD_SET_SIZE_BLOCKS, 
				num_blocks);
	
	// open thread
	m_nbd_thread = std::thread(&NBDDriverProxy::call_do_it, this);
}

NBDDriverProxy::~NBDDriverProxy()
{
	LOG(Logger::DEBUG, "dtor", __LINE__);
	if(!m_is_disconnected)
	{
		LOG(Logger::DEBUG, "calling Disconnect", __LINE__);
		call_ioctl("NBD_DISCONNECT", m_nbd_fd.get(), NBD_DISCONNECT);
	}
	
	m_nbd_thread.join();
	LOG(Logger::DEBUG, "thread joined", __LINE__);
} 

void NBDDriverProxy::setup(const std::string& dev_file)
{
	int sp[2];
	
	int status = socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
	if(-1 == status)
	{
		throw(DriverError("socketpair failed", strerror(errno)));
	}
	
	m_nbd_fd = open(dev_file.c_str(), O_RDWR);
	if(-1 == m_nbd_fd)
	{
		throw(DriverError("open nbd failed", strerror(errno)));
	}
	set_request_fd(sp[0]);
	m_nbd_socket = sp[1];
	
	#if defined NBD_SET_FLAGS
      int flags = 0;
	#if defined NBD_FLAG_SEND_TRIM
	  flags |= NBD_FLAG_SEND_TRIM;
	#endif
	#if defined NBD_FLAG_SEND_FLUSH
      flags |= NBD_FLAG_SEND_FLUSH;
	#endif
     if(0 != flags)
     {
     	call_ioctl("SET_FLAGS", m_nbd_fd.get(), NBD_SET_FLAGS, flags);
     } 
     #endif
	call_ioctl("NBD_CLEAR_SOCK", m_nbd_fd.get(), NBD_CLEAR_SOCK);
	
}

void NBDDriverProxy::call_do_it()
{
	try
	{
		sigset_t sigset;
		if(-1 == sigfillset(&sigset))
		{
			throw(DriverError("sigfillset failed", strerror(errno)));
		}
		if(-1 == pthread_sigmask(SIG_SETMASK, &sigset, NULL))
		{
			throw(DriverError("pthread_sigmask failed", strerror(errno)));
		}
		
		call_ioctl("NBD_SET_SOCK", m_nbd_fd.get(), NBD_SET_SOCK, m_nbd_socket.get());
		
		LOG(Logger::DEBUG, "calling DO_IT", __LINE__);
		
		call_ioctl("NBD_DO_IT", m_nbd_fd.get(), NBD_DO_IT); // blocking
		
		LOG(Logger::DEBUG, "return from DO_IT",	__LINE__);
	
		call_ioctl("NBD_CLEAR_QUE", m_nbd_fd.get(), NBD_CLEAR_QUE);
		call_ioctl( "NBD_CLEAR_SOCK", m_nbd_fd.get(), NBD_CLEAR_SOCK);
	}
	catch(DriverError& error)
	{
		LOG(Logger::DEBUG, std::string("error occured in thread ") + error.who() + 
		std::string(" ") + error.what(), __LINE__);
	}
}

std::unique_ptr<DriverData> NBDDriverProxy::receive_request()
{
	struct nbd_request request;
	int bytes_read = 0;
	DriverData::action_type type;
	std::string type_string;
	
	bytes_read = read(get_request_fd(), &request, sizeof(request));
	
	if(-1 == bytes_read)
	{
		LOG(Logger::ERROR, std::string(strerror(errno)), __LINE__);
		throw DriverError("read request failed", strerror(errno));
	}
	assert(sizeof(request) == bytes_read);
	
	std::size_t offset = ntohll(request.from);
	int length = ntohl(request.len);
	int req_type = ntohl(request.type);
	char* handle = request.handle;
	
	switch(req_type)
	{
		case NBD_CMD_READ:
		type = DriverData::READ;
		type_string = "read";
		break;
		
		case NBD_CMD_WRITE:
		type = DriverData::WRITE;
		type_string = "write";
		break;
		
		case NBD_CMD_DISC:
		type = DriverData::DISCONNECT;
		type_string = "disconnect";
		break;
		
		case NBD_CMD_FLUSH:
		type = DriverData::FLUSH;
		type_string = "flush";
		break;
		
		case NBD_CMD_TRIM:
		type = DriverData::TRIM;
		type_string = "trim";
		break;
	}
	
	std::unique_ptr<DriverData> 
					data(new NBDData(type, handle, offset, length));
	
	if(DriverData::WRITE == type)
	{
		std::vector<char>& req_buff = data->access_buffer();
		ProxyDriver::read_all(get_request_fd(), req_buff.data(), length);
	}
	
	std::ostringstream log_msg;
	log_msg << type_string << " request from device. offset: " << offset << " " 
	<< "length: " << length <<  " handle: " << handle;
	
	LOG(Logger::INFO, log_msg.str(), __LINE__);
	
	return data;
}

void NBDDriverProxy::send_reply(std::unique_ptr<DriverData> data)
{
	static const int HANDLE_SIZE = 8;
	struct nbd_reply reply;
	DriverData::action_type type = data->get_type();
	int len = data->get_length();
	
	reply.error = htonl(0);
	reply.magic = htonl(NBD_REPLY_MAGIC);
	
	NBDData* nbd_data = static_cast<NBDData*>(data.get());
	
	std::copy(nbd_data->get_handle(), nbd_data->get_handle() + HANDLE_SIZE, reply.handle);
	
	LOG(Logger::DEBUG, std::string("sending reply of handle: ") + 
	std::string(nbd_data->get_handle()), __LINE__);
	
	LockGuard<std::mutex> lock(m_mutex);
	ProxyDriver::write_all(get_request_fd(), reinterpret_cast<char*>(&reply), sizeof(reply));
	if(DriverData::READ == type)
	{
		std::vector<char>& req_buffer = nbd_data->access_buffer();
		ProxyDriver::write_all(get_request_fd(), req_buffer.data(), len);
	}
}

void NBDDriverProxy::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, std::string(RED "NBDDriver" RESET), msg, line);
}

void NBDDriverProxy::disconnect()
{
	LOG(Logger::DEBUG, "calling ioctl(disconnect)", __LINE__);
	call_ioctl("NBD_DISCONNECT", m_nbd_fd.get(), NBD_DISCONNECT);
	m_is_disconnected = true;
}
   
}   //namespace ilrd

