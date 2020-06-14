#include <sys/mman.h> // mmap
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <cstring> // strerror
#include <unistd.h> // truncate
#include <sstream> // ostringstream

#include "tools.hpp"
#include "exceptions.hpp" // DiskError
#include "mmap_storage.hpp"

namespace ilrd
{

MmapStorage::MmapStorage(const std::string& file_name, std::size_t size)
	: m_fd(open(file_name.c_str(), O_RDWR | O_CREAT, 0666)), m_logger(get_logger())
{
	if(-1 == m_fd)
	{
		LOG(Logger::ERROR, std::string("error opening file: ") + 
		std::string(strerror(errno)), __LINE__);
		
		throw DiskError(std::string("error opening file: ") + 
												std::string(strerror(errno)));
	}
	
	if(-1 == truncate(file_name.c_str(), size))
	{
		LOG(Logger::ERROR, std::string("truncate failed: ") + 
		std::string(strerror(errno)), __LINE__);
		throw DiskError(std::string("truncate failed: ") + 
												std::string(strerror(errno)));
	}
	
	void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
	if(nullptr == addr)
	{
		LOG(Logger::ERROR, std::string("mmap failed: ") + 
		std::string(strerror(errno)), __LINE__);
		throw DiskError(std::string("mmap failed: ") + 
												std::string(strerror(errno)));
	}
	
	m_base = reinterpret_cast<char*>(addr);
}

std::unique_ptr<DriverData> MmapStorage::read(std::unique_ptr<DriverData> data)
{
	std::size_t offset = data->get_offset();
	unsigned int length = data->get_length();
	
	std::copy(m_base + offset, m_base + offset + length, 
			  data->access_buffer().data());
	
	std::ostringstream msg;
	msg << "reading from disk: offset: " << offset << " length: " << length;
	LOG(Logger::INFO, msg.str(), __LINE__);
	
	return data;
}

std::unique_ptr<DriverData> MmapStorage::write(std::unique_ptr<DriverData> data)
{
	std::size_t offset = data->get_offset();
	unsigned int length = data->get_length();
	std::vector<char>& buffer = data->access_buffer();
	
	std::copy(buffer.data(), buffer.data() + length, m_base + offset);
	
	std::ostringstream msg;
	msg << "writing to disk: offset: " << offset << " length: " << length;
	LOG(Logger::INFO, msg.str(), __LINE__);
	
	return data;
}

void MmapStorage::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, CYANDARK "MmapStorage" RESET, msg, line);
}


} // ilrd
