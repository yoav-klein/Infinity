
#include "udp_gate.hpp"


namespace ilrd
{

UDPGate::UDPGate(ProxyDriver* driver, Storage* storage)
	: GateWay<int, DataArgs>(driver->get_request_fd()),  m_driver(driver), 
	   m_storage(storage)
{
}

std::pair<int, DataArgs> UDPGate::read()
{
	std::unique_ptr<DriverData> request = m_driver->receive_request();
	int type = request->get_type();
	
	return std::pair<int, DataArgs>(type, DataArgs(m_driver, m_storage, 
									 std::move(request)));
}	





} // ilrd
