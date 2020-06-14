#include <iostream> // cout
#include <string.h> // strerror
#include <unistd.h> // read
#include <assert.h> // assert

#include "exceptions.hpp"
#include "driver_proxy.hpp"

namespace ilrd
{

ProxyDriver::~ProxyDriver() { } 

int ProxyDriver::get_request_fd() const
{
 	return m_request_fd;
} 
    
void ProxyDriver::set_request_fd(int fd)
{
   	m_request_fd = fd;
}

void ProxyDriver::read_all(int fd, char* buf, size_t count)
{
	int bytes_read;
	
	while (count > 0)
	{
		bytes_read = read(fd, buf, count);
		if(-1 == bytes_read)
		{
			throw DriverError("read_all: ", strerror(errno));
		}
		buf += bytes_read;
		count -= bytes_read;
  	}
  	
  	assert(count == 0);
}

void ProxyDriver::write_all(int fd, const char* buf, size_t count)
{
  	int bytes_written;

  	while (count > 0) 
  	{
		bytes_written = write(fd, buf, count);
		if(-1 == bytes_written)
		{
			throw DriverError("write_all: ", strerror(errno));
		}
		buf += bytes_written;
		count -= bytes_written;
  	}
  	
	assert(count == 0);
}

}   //namespace ilrd

