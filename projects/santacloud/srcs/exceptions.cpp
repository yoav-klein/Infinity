#include "exceptions.hpp"

namespace ilrd
{

// NBDError ctor
DriverError::DriverError(const std::string& orig, const char* msg)
	: std::runtime_error(msg), m_orig(orig) 
{ }

const std::string& DriverError::who()
{
	return m_orig;
}


DiskError::DiskError(std::string msg)
	: std::runtime_error(msg) { }



BadKey::BadKey(std::string str)
		: std::logic_error(str) { }



BadCreate::BadCreate()
		: std::bad_alloc() { } 

} // ilrd
