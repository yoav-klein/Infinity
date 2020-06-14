
/*************************************************************************
	EpollWrapper class
	a derived class from Monitor
	
	wraps the epoll operations
	some functions may throw std::runtime_error
	
	The class holds a vector of epoll_event type
	the size of this vector is received and defined
	in the ctor, stays constant.
	
	the function add receives as parameter the enum "event" which 
	specifies what type of I/O should the epoll 'listen' to.
	this should be improved to receive a variable number of these
	types, since that's what epoll allows. see man epoll_ctl
	
*************************************************************************/

#ifndef __EPOLL_WRAPPER_HPP__
#define __EPOLL_WRAPPER_HPP__

#include <vector> // std::vector
#include <sys/epoll.h> // epoll_create

#include "file_raii.hpp" // FileDescriptor
#include "monitor.hpp" // Monitor
#include "logger.hpp"  // Logger

namespace ilrd
{


class EpollWrapper : public Monitor
{
public:
	EpollWrapper(std::size_t size_of_events_arr, size_t timeout_secs);
	~EpollWrapper() override;
	
	void add(int fd, Monitor::event type) override;
	void remove(int fd) override;
	int wait() override;
	Event operator[](std::size_t index) override;
	
private:
	const std::size_t m_timeout;
	FileDescriptor m_epfd;
	std::vector<struct epoll_event> m_events;
	Logger* m_logger;
	void LOG(Logger::Severity sever, const std::string& msg, int line);
};

}

#endif // __EPOLL_WRAPPER_HPP__
