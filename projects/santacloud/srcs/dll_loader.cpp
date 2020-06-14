#include <algorithm> // for_each
#include <dlfcn.h> // dlopen

#include "tools.hpp"
#include "handleton.hpp" // get_logger
#include "dll_loader.hpp"


namespace ilrd
{

Logger* get_logger(); // in libglob.so

DllLoader::DllLoader(Dispatcher<std::string>* dispatcher)
	: m_callback(dispatcher, *this, &DllLoader::dll_load, &DllLoader::on_stop),
	  m_logger(get_logger())
{
}

DllLoader::~DllLoader()
{
	for(auto iter : m_handles)
	{
		std::cout << "dlclosing: " << iter.second << std::endl;
		if(-1 == dlclose(iter.second))
		{
			m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, 
			std::string("couldn't dlclose") + iter.first, __LINE__);
		}
	}
	m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, "Dtor", __LINE__);
}

void DllLoader::dll_load(const std::string& msg)
{
	m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, 
	std::string("received name") + msg, __LINE__);
	
	// first check if library is already loaded. if is, close and delete
	std::unordered_map<std::string, void*>::iterator iter = m_handles.find(msg);
	if(iter != m_handles.end())
	{
		m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, 
			std::string("library already loaded") + msg, __LINE__);
		if(-1 == dlclose((*iter).second))
		{
			m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, 
			std::string("couldn't dlclose") + msg, __LINE__);
		}
		else
		{
			m_handles.erase(msg);
		}
	}
	
	void* handle = dlopen(msg.c_str(), RTLD_LAZY);
	if(nullptr == handle)
	{
		m_logger->write(Logger::ERROR, ORANGE "DllLoader" RESET , 
		   				"error loading library", __LINE__);
		
	}
	else
	{
		m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, "library loaded", 
						__LINE__);
		
		std::cout << "inserting " << handle << std::endl;
		m_handles.insert({msg, handle});
	}
}

void DllLoader::on_stop()
{
	m_logger->write(Logger::DEBUG, ORANGE "DllLoader" RESET, "monitor quit", __LINE__);
}


} // ilrd

