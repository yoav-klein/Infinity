#include <iostream> // cout
#include <algorithm> // copy

#include <driver_data.hpp>

namespace ilrd
{

DriverData::DriverData(action_type type, size_t offset,
                       unsigned int length, int req_id)	
	: m_type(type), m_offset(offset), m_length(length), m_req_id(req_id),
	  m_buffer(length)
{
}

DriverData::action_type DriverData::get_type() const 
{
	return m_type;	 
}

std::size_t DriverData::get_offset() const 
{ 
	return m_offset;
}

unsigned int DriverData::get_length() const 
{ 
	return m_length;
}

int DriverData::get_req_id()
{
	return m_req_id;
}	

void DriverData::set_req_id(int req_id)
{
	m_req_id = req_id;
}

void DriverData::set_status(int status)
{
	m_status = status;
}

std::vector<char>& DriverData::access_buffer()
{
	return m_buffer;
}

}   //namespace ilrd

