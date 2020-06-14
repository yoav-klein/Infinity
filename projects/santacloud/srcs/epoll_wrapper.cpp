#include <strings.h> // bzero
#include <unistd.h> // close


#include "epoll_wrapper.hpp"
#include "handleton.hpp" // for Logger

#include "tools.hpp"


namespace ilrd
{

EpollWrapper::EpollWrapper(std::size_t size_of_events_arr, std::size_t timeout)
	: m_timeout(timeout * 1000), m_events(size_of_events_arr), 
	  m_logger(Handleton<Logger>::get_instance())
{ 
	m_epfd = epoll_create(10); // 10 is a random number since the argument is ignored
	if(-1 == m_epfd)
	{
		throw std::runtime_error("epoll_create failed");
	}
}

EpollWrapper::~EpollWrapper() 
{ 
}

void EpollWrapper::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, "EpollWrapper", msg, line);
}

void EpollWrapper::add(int fd, Monitor::event event)
{ 
	struct epoll_event epoll_event;
	bzero(&epoll_event, sizeof(epoll_event));
	epoll_event.data.fd = fd;
	if(Monitor::READ == event)
	{
		epoll_event.events = EPOLLIN;
	}
	if(Monitor::WRITE == event)
	{
		epoll_event.events = EPOLLOUT;
	}
	
	if(-1 == epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &epoll_event))
	{
		LOG(Logger::ERROR, "epoll_ctl ADD failed", __LINE__);
		throw(std::runtime_error("epoll_ctl ADD failed"));
	}
	
}

void EpollWrapper::remove(int fd)
{
	if(-1 == epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL))
	{
		throw(std::runtime_error("epoll_ctl DEL failed"));
	} 
}

int EpollWrapper::wait()
{ 
	int nfds = epoll_wait(m_epfd, m_events.data(), m_events.capacity(), m_timeout);
	if(-1 == nfds) 
	{
		DEBUG_ONLY(perror("epoll_wait"));  
		if(errno != EINTR)    // in case errno is set to this value, it means
		{     				// that the wait was interrupted by a signal. in our
			throw(std::runtime_error("epoll_wait failed")); // case it means 
		} // ctrl+c which is fine so we don't want to throw. maybe there's 
	}		// a better way to handle this.
	
	return nfds;
}

Event EpollWrapper::operator[](std::size_t index)
{	
	int fd = m_events[index].data.fd;
	Monitor::event event;
	switch(m_events[index].events)
	{
		case EPOLLIN:
			event = READ;
			break;
		case EPOLLOUT:
			event = WRITE;
			break;
		case EPOLLIN | EPOLLHUP:
			event = HUP;
			break;
		default:
			event = READ;
			break;
	}
	
	return Event(fd, event);
	
}


} // namespace ilrd
