/************************************************
	StdinGate
	
	a derived class from GateWay<int, MasterArgs>
	it's function is to read input from stdin,
	and if 'q' was pressed, return '6' as key
	to the factory. 
	
	the create function to '6' is a function that
	creates a task that calls 'disconnect'
	

***************************************************/


#ifndef __STDIN_GATE_HPP__
#define __STDIN_GATE_HPP__

#include <utility> // std::pair
#include "gateway.hpp" // GateWay
#include "master_args.hpp" // MasterArgs
#include "driver_proxy.hpp"

namespace ilrd
{

static const int STOP_KEY = 10; // TODO: maybe enum?
static const int DO_NOTHING_KEY = 11;

class StdinGate : public GateWay<int, MasterArgs>
{
public:
	StdinGate(ProxyDriver* driver);
	
	std::pair<int, MasterArgs> read();
	
private:
	ProxyDriver* m_driver;

};





} // ilrd



#endif // __STDIN_GATE_HPP__
