#ifndef __MASTER_ARGS_HPP__
#define __MASTER_ARGS_HPP__

#include "kamikaze.hpp" // Storage
#include "driver_proxy.hpp" // ProxyDriver


namespace ilrd
{

struct MasterArgs
{
	MasterArgs(ProxyDriver* driver, Kamikaze* storage, 
				std::unique_ptr<DriverData> data);
	
	MasterArgs(const MasterArgs& other) = delete;
	MasterArgs& operator=(const MasterArgs& other) = delete;
	
	MasterArgs(MasterArgs&& other);
	MasterArgs& operator=(MasterArgs&&) = default;
	
	ProxyDriver* m_driver;
	Kamikaze* m_kamikaze;
	std::unique_ptr<DriverData> m_data;
};


} // ilrd

#endif // __MASTER_ARGS_HPP__
