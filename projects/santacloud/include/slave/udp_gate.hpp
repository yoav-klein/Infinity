/******************************************************
	UDPGate
	
	this class is a derived from GateWay<int, SlaveArgs>,
	it's read function returns a pair of int, which is
	a key to the factory (read or write task), and SlaveArgs
	to be passed to the create function.
	
	Author: Yoav Klein
	Date: 9.12.19

********************************************************/


#ifndef __UDP_GATE_HPP__
#define __UDP_GATE_HPP__

#include <utility> // std::pair
#include "storage.hpp" // Storage
#include "driver_proxy.hpp" // ProxyDriver
#include "gateway.hpp" // GateWay
#include "data_args.hpp" // DataArgs

namespace ilrd
{

class UDPGate : public GateWay<int, DataArgs>
{
public:
	UDPGate(ProxyDriver* driver, Storage* storage);

	std::pair<int, DataArgs> read() override;

private:
	ProxyDriver* m_driver;
	Storage* m_storage;
};







} // ilrd


#endif // __UDP_GATE_HPP__
