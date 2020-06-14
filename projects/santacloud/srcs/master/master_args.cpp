
#include "master_args.hpp"


namespace ilrd
{


MasterArgs::MasterArgs(ProxyDriver* driver, Kamikaze* kamikaze, 
				       std::unique_ptr<DriverData> data)
	: m_driver(driver), m_kamikaze(kamikaze), m_data(std::move(data))
{
}

MasterArgs::MasterArgs(MasterArgs&& other)
	: m_driver(other.m_driver), m_kamikaze(other.m_kamikaze), 
	  m_data(std::move(other.m_data))
{
}






} // ilrd
