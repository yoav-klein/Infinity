
#include "nbd_gate.hpp" 


namespace ilrd
{

NBDGate::NBDGate(ProxyDriver* driver, Kamikaze* kamikaze)
	: GateWay<int, MasterArgs>(driver->get_request_fd()), 
	m_driver(driver), m_kamikaze(kamikaze)
{
}

// SendRequestTask is created by the function coresponding to the key 
// with value 0 (DriverData::READ), so, if the request type is READ or WRITE,
// type will be set to DriverData::READ, so a SendRequestTask will be generated.
// any other request (FLUSH, DISCONNECT, TRIM) will generate DiscTask or OtherTask.
std::pair<int, MasterArgs> NBDGate::read()
{
	std::unique_ptr<DriverData> request = m_driver->receive_request();
	
	DriverData::action_type type = request->get_type();
	
	if((type == DriverData::READ) | (type == DriverData::WRITE))
	{
		type = DriverData::READ;
	}
	std::pair<int, MasterArgs> pair(type, 
						MasterArgs(m_driver, m_kamikaze, std::move(request)));
	
	return pair;
}



}


