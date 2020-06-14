#ifndef __DLL_LOADER_HPP__
#define __DLL_LOADER_HPP__

/*************************************************
	DllLoader
	
	this class is part in the mechanism of Plug&Play. 
	DirMonitor sends notifications once there's 
	modifications in the directory monitored by it.
	
	once notified, DllLoader loads the library explicitly.


****************************************************/


#include <string> // std::string
#include <unordered_map> // std::unordered_map

#include "logger.hpp" // Logger
#include "callback_dispatcher.hpp" // Callback/Dispatcher

namespace ilrd
{

class DllLoader
{
public:
    explicit DllLoader(Dispatcher<std::string>* dispatcher);

    ~DllLoader();

    void dll_load(const std::string& msg);
    void on_stop();

    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;
    DllLoader(DllLoader&&) = delete;
    DllLoader& operator=(DllLoader&&) = delete;

private:
    Callback<std::string, DllLoader> m_callback;
	std::unordered_map<std::string, void*> m_handles;
	Logger* m_logger;
};

} //ilrd


#endif //__DLL_LOADER_HPP__ 
