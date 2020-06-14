/**********************************************************
	SlaveGate

	this is a gateway for the slave sockets.
	it's read() function reads the reply from the slave,
	finds the req_id in the map and returns the corresponding
	std::unique_ptr<DriverData>

	Author: Yoav Klein
	Date: 25.12.19

**************************************************************/

#ifndef __SLAVE_GATE_HPP__
#define __SLAVE_GATE_HPP__

#include "logger.hpp" // Logger
#include "gateway.hpp" // GateWay
#include "nbd_driver.hpp" // ProxyDriver
#include "master_tasks.hpp" // MasterArgs

namespace ilrd
{

class SlaveGate : public GateWay<int, MasterArgs>
{
public:
	SlaveGate(int fd, ProxyDriver* driver, Kamikaze* kamikaze);
	
	std::pair<int, MasterArgs> read() override;

private:
	ProxyDriver* m_driver;
	Kamikaze* m_kamikaze;
	
}; 

} // ilrd




#endif // __SLAVE_GATE_HPP__
