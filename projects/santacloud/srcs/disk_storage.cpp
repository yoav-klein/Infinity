#include <cstring> // strerror
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open

#include <unistd.h> // truncate
#include <sstream> // ostringstream

#include "logger.hpp" // write
#include "lockguard.hpp" // Lock
#include "tools.hpp"

#include "disk_storage.hpp"

namespace ilrd
{

Logger* get_logger(); // in globals.cpp

int read_all(int fd, char* buf, std::size_t length)
{
	int bytes_read = 0;
	
	while(length)
	{
		bytes_read = read(fd, buf, length);
		if(-1 == bytes_read)
		{
			return -1;
		}
		buf += bytes_read;
		length -= bytes_read;
	}
	return 0;
}

int write_all(int fd, const char* buf, std::size_t length)
{
	int bytes_written = 0;
	
	while(length)
	{
		bytes_written = write(fd, buf, length);
		if(-1 == bytes_written)
		{
			return -1;
		}
		buf += bytes_written;
		length -= bytes_written;
	}
	return 0;
}


DiskStorage::DiskStorage(std::string file_name, std::size_t size)
	: m_fd(open(file_name.c_str(), O_RDWR)), m_logger(get_logger())
{
	if(-1 == m_fd)
	{
		m_logger->write(Logger::ERROR, "DiskStorage", strerror(errno), __LINE__);
		throw DiskError(std::string("couldn't open file: ") + 
												std::string(strerror(errno)));
	}
	if(-1 == truncate(file_name.c_str(), size))
	{
		m_logger->write(Logger::ERROR, "DiskStorage", strerror(errno), __LINE__);
		throw DiskError(std::string("truncate failed: ") + 
												std::string(strerror(errno)));
	}
}

std::unique_ptr<DriverData> DiskStorage::read(std::unique_ptr<DriverData> data)
{
	std::vector<char>& request_buffer = data->access_buffer();
	std::size_t offset = data->get_offset();
	int length = data->get_length();
	
	std::ostringstream msg;
	msg << "reading from disk: offset: " << offset << " length: " << length;
	m_logger->write(Logger::DEBUG, GREEN "DiskStorage" RESET, msg.str(), __LINE__);
	
	LockGuard<std::mutex> lock(m_mutex);
	lseek(m_fd, offset, SEEK_SET);
	
	int status = read_all(m_fd, request_buffer.data(), length);
	if(-1 == status)
	{
		m_logger->write(Logger::ERROR, GREEN "DiskStorage" RESET, 
		std::string(RED "ERROR: ") + std::string(strerror(errno)), __LINE__);
	}
	
	data->set_status(status);
	
	return data;
}

std::unique_ptr<DriverData> DiskStorage::write(std::unique_ptr<DriverData> data)
{
	std::vector<char>& request_buffer = data->access_buffer();
	std::size_t offset = data->get_offset();
	int length = data->get_length();
	
	std::ostringstream msg;
	msg << "writing to disk: offset: " << offset << " length: " << length;
	m_logger->write(Logger::DEBUG, GREEN "DiskStorage" RESET, msg.str(), __LINE__);
	
	LockGuard<std::mutex> lock(m_mutex);
	lseek(m_fd, offset, SEEK_SET);
	int status = write_all(m_fd, request_buffer.data(), length);
	
	if(-1 == status)
	{
		m_logger->write(Logger::ERROR, GREEN "DiskStorage" RESET, 
		std::string(RED "ERROR: ") + std::string(strerror(errno)), __LINE__);
	}
	
	data->set_status(status);
	
	return data;
}

} // ilrd
