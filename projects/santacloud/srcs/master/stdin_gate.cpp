#include <unistd.h> // read
#include "stdin_gate_master.hpp"


namespace ilrd
{

StdinGate::StdinGate(ProxyDriver* driver)
	: GateWay<int, MasterArgs>(0), m_driver(driver)
{

}

// read from stdin, if 'q' is pressed, return a key that'll generate
// a task that'll call disconnect.
// else, reutrn a key that'll generate a DoNothingTask

std::pair<int, MasterArgs> StdinGate::read()
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
	
	return std::pair<int, MasterArgs>(ret_val, MasterArgs(m_driver, nullptr, nullptr));
}









} // ilrd
