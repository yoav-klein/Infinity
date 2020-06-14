#ifndef __REQUEST_ENGINE_H__
#define __REQUEST_ENGINE_H__

 
 /**************************************************************
 	
 	Request Engine
 	
 	this class wraps together the ThreadPool and Factory mainly.
 	the main loop, run(), waits on epoll. each fd in the epoll has 
 	a GateWay, through which it carries out the actions to be done 
 	when this fd signals. this GateWay then returns a key and arguments
 	to be passed to the factory, which creates a task to be pushed to the
 	ThreadPool.
 	
 	!!NOTICE: the Factory is templated on <RETask, Key, Args, !!bool&!!>
 	this bool& is the m_should_stop, which is the condition for the main loop.
 	that means that creator functions that are added to the Factory 
 	always must receive a bool& for their last argument. (the second actually)

 *****************************************************************/

#include <unordered_map> // unordered_map
#include <sstream> // ostringstream

#include "handleton.hpp" // Handleton<Factory>
#include "factory.hpp" // Factory<RETask, ... >
#include "thread_pool.hpp" // ThreadPool
#include "dir_monitor.hpp" // DirMonitor
#include "dll_loader.hpp" // DllLoader
#include "retask.hpp" // RETask
#include "gateway.hpp" // GateWay
#include "monitor.hpp" // Monitor
#include "logger.hpp" // Logger
#include "epoll_wrapper.hpp" // EpollWrapper
#include "tools.hpp"

namespace ilrd
{

template <typename Key, typename Args>
class RequestEngine final
{
public:
    //if hardware_concurrency() not supported, default to 4
    explicit RequestEngine(const std::string& monitor_path, size_t num_of_threads = std::thread::hardware_concurrency());
    ~RequestEngine();

    RequestEngine(const RequestEngine& other) = delete;
    RequestEngine& operator=(const RequestEngine& other) = delete;
    RequestEngine(const RequestEngine&& other) = delete;
    RequestEngine& operator=(const RequestEngine&& other) = delete;
    
    using CreatorFunc = std::function<std::unique_ptr<RETask>(Args args, bool&)>; 

    void add_gateway(GateWay<Key, Args>* gateway);
    void add_task(const Key& key, CreatorFunc creator);
    void set_threads_num(size_t num);
    
    void run();

private:
    Factory<RETask, Key, Args, bool&>* m_factory;
    ThreadPool m_pool;
    std::unordered_map<int, GateWay<Key, Args>*> m_gateways;
    std::unique_ptr<Monitor> m_monitor;
    DirMonitor m_dir_monitor; //-wrap together in class p&p: optional
    DllLoader m_dll_loader;
   	bool m_should_stop;  
   	
   	Logger* m_logger;
   	void LOG(Logger::Severity sever, const std::string& msg, int line);  	
};

template <typename Key, typename Args>
RequestEngine<Key, Args>::RequestEngine(const std::string& monitor_path, size_t 										num_of_threads)
	: m_factory(Handleton<Factory<RETask, Key, Args, bool&> >::get_instance()),
	  m_pool(num_of_threads > 0 ? num_of_threads : std::thread::hardware_concurrency()), 
	  m_monitor(new EpollWrapper(10, -1)),
	  m_dir_monitor(monitor_path), m_dll_loader(m_dir_monitor.get_dispatcher()), 
	  m_should_stop(false), m_logger(Handleton<Logger>::get_instance())
{
	LOG(Logger::DEBUG, "Ctor", __LINE__);
}

template <typename Key, typename Args>
RequestEngine<Key, Args>::~RequestEngine()
{ 
	LOG(Logger::DEBUG, "dtor", __LINE__);
}

template <typename Key, typename Args>
void RequestEngine<Key, Args>::add_gateway(GateWay<Key, Args>* gateway)
{
	int fd = gateway->get_fd();
	
	m_gateways.insert({fd, gateway});
	m_monitor->add(fd, Monitor::READ);
}

template <typename Key, typename Args>
void RequestEngine<Key, Args>::add_task(const Key& key, CreatorFunc creator)
{
	m_factory->add(key, creator);
}

template <typename Key, typename Args>
void RequestEngine<Key, Args>::set_threads_num(size_t num)
{
	m_pool.set_size(num);
}

template <typename Key, typename Args>
void RequestEngine<Key, Args>::run()
{
	while(!m_should_stop)
	{
		LOG(Logger::DEBUG, "before epoll wait", __LINE__);
		int nfds = m_monitor->wait();
		
		for(int i = 0; i < nfds; ++i)
		{
			std::ostringstream msg;
			msg << "epoll_wait returns with: " << (*m_monitor)[i].get_fd();
			LOG(Logger::DEBUG, msg.str(), __LINE__);
			if((*m_monitor)[i].get_event() != Monitor::HUP) // since epoll returns on 
			{									// hangup, e.g. socket closing 
				std::pair<Key, Args> pair = 
				m_gateways[(*m_monitor)[i].get_fd()]->read();
				std::unique_ptr<RETask> task = 
				m_factory->create(pair.first, std::move(pair.second), 
								  m_should_stop);
			
				m_pool.add_task(std::move(task));
			}
			else
			{
				m_monitor->remove((*m_monitor)[i].get_fd());
			}
		}
		
	}
}

template <typename Key, typename Args>
void RequestEngine<Key, Args>::LOG(Logger::Severity sever, const std::string& msg, 
									int line)
{
	m_logger->write(sever, "RequestEngine", msg, line);
}
}//namespace ilrd

#endif //__REQUEST_ENGINE_H__
