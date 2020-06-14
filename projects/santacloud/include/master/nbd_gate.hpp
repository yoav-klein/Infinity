/*******************************************************

	NBDGate
	
	a derived class from GateWay<int, MasterArgs>
	it's function is to read requests from NBDDriver
	and return a pair of int (which is the key to the
	create function of Factory<RETask, int, MasterArgs..>
	and MasterArgs, which contains the Storage*, Driver* and DriverData
	to create the task.
	
	Author: Yoav Klein
	Date: 5.12.19
	

***********************************************/

#ifndef __NBD_GATE_HPP__
#define __NBD_GATE_HPP__

#include "logger.hpp" // Logger
#include "nbd_driver.hpp" // ProxyDriver
#include "master_tasks.hpp" // MasterArgs
#include "kamikaze.hpp" // Kamikaze
#include "gateway.hpp"

namespace ilrd
{

class NBDGate : public GateWay<int, MasterArgs>
{
public:
	NBDGate(ProxyDriver* driver, Kamikaze* kamikaze);
	
	std::pair<int, MasterArgs> read() override;

private:
	ProxyDriver* m_driver;
	Kamikaze* m_kamikaze;

};

} // ilrd


#endif //__NBD_GATE_HPP__
