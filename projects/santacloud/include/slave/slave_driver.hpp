/***************************************************
	
	SlaveDriver
	
	A derived class from ProxyDriver.
	used in Slaves to read requests from socket
	connected to master.

*****************************************************/


#ifndef __SLAVE_DRIVER_HPP__
#define __SLAVE_DRIVER_HPP__

#include <mutex> // std::mutex

#include "logger.hpp" 
#include "driver_proxy.hpp"

namespace ilrd
{

class SlaveDriver : public ProxyDriver
{
public:
	SlaveDriver();
	
	std::unique_ptr<DriverData> receive_request() override;  
    void send_reply(std::unique_ptr<DriverData> data) override;
    void disconnect() override;
    
    void ping_master();

private:
	const std::size_t MAX_MSG_SIZE;
	static const std::size_t MAX_HEADER_SIZE = 40;

	std::mutex m_mutex;
	Logger* m_logger;
	void LOG(Logger::Severity sever, const std::string& msg, int line);
};




}




#endif // __SLAVE_DRIVER_HPP__
