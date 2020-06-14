#include "do_nothing_master.hpp" // DoNothingTask
#include "stdin_gate_master.hpp" // DO_NOTHING_KEY

#include "slave_gate.hpp"


namespace ilrd
{

SlaveGate::SlaveGate(int fd, ProxyDriver* driver, Kamikaze* kamikaze)
	: GateWay<int, MasterArgs>(fd), m_driver(driver), m_kamikaze(kamikaze)
{
}

std::pair<int, MasterArgs> SlaveGate::read()
{
	std::unique_ptr<DriverData> reply = m_kamikaze->receive_reply(get_fd());
	
	if(reply == nullptr)
	{
		return std::pair<int, MasterArgs>(DO_NOTHING_KEY, MasterArgs(nullptr,
										  nullptr, nullptr));
	}
	std::pair<int, MasterArgs> pair(1, 
						MasterArgs(m_driver, m_kamikaze, std::move(reply)));
	
	LOG(Logger::DEBUG, "passing request", __LINE__);
	
	return pair; 
}



} // ilrd
