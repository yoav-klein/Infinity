#include <unistd.h> // read
#include "stdin_gate_slave.hpp"


namespace ilrd
{

StdinGate::StdinGate(ProxyDriver* driver)
	: GateWay<int, DataArgs>(0), m_driver(driver)
{

}

std::pair<int, DataArgs> StdinGate::read()
{
	std::string input;
	char c = 0;
	int ret_val = 0;
	
	// STDIN_FILENO
	::read(0, &c, 1);
	while(c != '\n')
	{
		input += c;
		::read(0, &c, 1);		
	}
	
	if("q" == input)
	{
		ret_val = STOP_KEY;
	}
	else
	{
		ret_val = DO_NOTHING_KEY;
	}
	
	return std::pair<int, DataArgs>(ret_val, DataArgs(m_driver, nullptr, nullptr));
	 
}









} // ilrd
