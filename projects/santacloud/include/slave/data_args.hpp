#ifndef __DATA_ARGS_HPP__
#define __DATA_ARGS_HPP__

#include "storage.hpp" // Storage
#include "driver_proxy.hpp" // ProxyDriver


namespace ilrd
{

struct DataArgs
{
	DataArgs(ProxyDriver* driver, Storage* storage, 
				std::unique_ptr<DriverData> data);
	
	DataArgs(const DataArgs& other) = delete;
	DataArgs& operator=(const DataArgs& other) = delete;
	
	DataArgs(DataArgs&& other);
	DataArgs& operator=(DataArgs&&) = default;
	
	ProxyDriver* m_driver;
	Storage* m_storage;
	std::unique_ptr<DriverData> m_data;
};


} // ilrd

#endif // __DATA_ARGS_HPP__
