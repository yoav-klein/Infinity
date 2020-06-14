#ifndef __DIR_MONITOR_HPP__
#define __DIR_MONITOR_HPP__

/***********************************************************
	DirMonitor
	
	this class' target is to monitor a directory
	that contains shared objects, and report
	to another module - "DllLoader" - about 
	the new/modified shared objects in the directory, so it 
	could load/reload it.
	
	the object opens a thread that is blocked while 'listening' to
	the directory.
	
	this mechanism uses the Dispatcher-Callback modules.

	Author: Yoav Klein
	Date: 3.12.19
	
***********************************************************/

#include <string> // std::string

#include <thread> // std::thread
#include "callback_dispatcher.hpp"
#include "logger.hpp"

namespace ilrd
{

class DirMonitor
{
public:
    explicit DirMonitor(const std::string& path);
    ~DirMonitor();

    Dispatcher<std::string>* get_dispatcher();

    DirMonitor(const DirMonitor&) = delete;
    DirMonitor& operator=(const DirMonitor&) = delete;
    DirMonitor(DirMonitor&&) = delete;
    DirMonitor& operator=(DirMonitor&&) = delete;

private:
	bool m_should_run;
	int m_fd;
    void monitor(); 
    Dispatcher<std::string> m_dispatcher;
    std::string m_path; 
    std::thread m_thread;
	Logger* m_logger;
};

} //ilrd


#endif //__DIR_MONITOR_HPP__
