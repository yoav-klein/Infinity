
#include "nbd_data.hpp"


namespace ilrd
{

NBDData::NBDData(action_type type, char* handle, size_t offset, 
				 unsigned int length, int req_id)
	: DriverData(type, offset, length, req_id)
{
	std::copy(handle, handle + HANDLE_SIZE, m_handle);
}

char* NBDData::get_handle()
{
	return m_handle;
}


} // ilrd
