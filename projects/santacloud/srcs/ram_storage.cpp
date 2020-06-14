
#include <iostream> // std::cout
#include <algorithm> // std::transform

#include "tools.hpp" // colors defines
#include "logger.hpp" // write

#include "ram_storage.hpp"

namespace ilrd
{

Storage::~Storage() { }

RAMStorage::RAMStorage(std::size_t size)
	: m_buffer(size) { }

RAMStorage::~RAMStorage() { }

std::unique_ptr<DriverData> 
					RAMStorage::read(std::unique_ptr<DriverData> data)
{
	std::vector<char>& request_buffer = data->access_buffer();
	std::size_t offset = data->get_offset();
	int length = data->get_length();
	
	std::copy(m_buffer.begin() + offset, m_buffer.begin() + offset + length,
				request_buffer.begin());

	DEBUG_ONLY(std::cout << GREEN "RAMStorage: read request of length: " << 
	length << " offset: " << offset << " done" RESET << std::endl;) 
	
	return data;
}

std::unique_ptr<DriverData> 
					RAMStorage::write(std::unique_ptr<DriverData> data)
{
	std::vector<char>& request_buffer = data->access_buffer();
	std::size_t offset = data->get_offset();
	int length = data->get_length();
	
	std::copy(request_buffer.begin(), request_buffer.end(), 
				m_buffer.begin() + offset);
	
	DEBUG_ONLY(std::cout << GREEN "RAMStorage: write request of length: " << 
	length << " offset: " << offset << " done" RESET << std::endl;)
	
	return data;
}


}   //namespace ilrd

