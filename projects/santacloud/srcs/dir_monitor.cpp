#include <sys/inotify.h> // inotify_init
#include <unistd.h> // read

#include "tools.hpp"
#include "epoll_wrapper.hpp" // EpollWrapper
#include "dir_monitor.hpp"

namespace ilrd
{

Logger* get_logger();

DirMonitor::DirMonitor(const std::string& path)
	: m_should_run(true), m_path(std::move(path)), m_logger(get_logger())
{
	m_fd = inotify_init();
	if(-1 == m_fd)
	{
		m_logger->write(Logger::ERROR, ORANGE "DirMonitor" RESET, 
		"inotify_init failed", __LINE__);
		throw std::runtime_error("inotify_init failed");
	}
	
	if(-1 == inotify_add_watch(m_fd, path.c_str(), IN_CREATE | IN_CLOSE_WRITE))
	{
		m_logger->write(Logger::ERROR, ORANGE "DirMonitor" RESET, 
		"inotify_add_watch failed", __LINE__);
		throw std::runtime_error("inotify_add_watch failed");
	}
	
	m_logger->write(Logger::DEBUG, ORANGE "DirMonitor" RESET, 
		"starting thread", __LINE__);
	
	m_thread = std::thread(&DirMonitor::monitor, this);
	
}

DirMonitor::~DirMonitor()
{
	close(m_fd);
	m_logger->write(Logger::DEBUG, ORANGE "DirMonitor" RESET, 
		"Dtor", __LINE__);
	m_should_run = false;
	
	m_thread.join();
	m_logger->write(Logger::DEBUG, ORANGE "DirMonitor" RESET, 
		"thread joined", __LINE__);
}

Dispatcher<std::string>* DirMonitor::get_dispatcher()
{
	return &m_dispatcher;
}

void DirMonitor::monitor()
{
	char *runner;
	const std::size_t TIMEOUT = 10;
	
	EpollWrapper epoll(1, TIMEOUT);
	epoll.add(m_fd, Monitor::READ); 
	
	const std::size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	while(m_should_run)
	{
		int nfds = epoll.wait();
		
		if(nfds > 0) // wait returned not because of timeout
		{
			int read_bytes = read(m_fd, buffer, BUFFER_SIZE);
			for(runner = buffer; runner < buffer + read_bytes; runner += 
			 sizeof(struct inotify_event) + ((struct inotify_event*)runner)->len)
			{
				struct inotify_event* event = (struct inotify_event*)runner;
				m_logger->write(Logger::DEBUG, ORANGE "DirMonitor" RESET, 
		std::string("file detected: ") + std::string(event->name), __LINE__);
				m_dispatcher.notify_all(m_path + "/" + std::string(event->name));
			}
		}

	}

}


} // ilrd
