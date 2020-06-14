
#include "data_args.hpp"


namespace ilrd
{


DataArgs::DataArgs(ProxyDriver* driver, Storage* storage, 
				       std::unique_ptr<DriverData> data)
	: m_driver(driver), m_storage(storage), m_data(std::move(data))
{
}

DataArgs::DataArgs(DataArgs&& other)
	: m_driver(other.m_driver), m_storage(other.m_storage), 
	  m_data(std::move(other.m_data))
{
}






} // ilrd
