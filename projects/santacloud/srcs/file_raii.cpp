
#include <unistd.h> // close

#include "file_raii.hpp"
namespace ilrd
{

FileDescriptor::FileDescriptor(int fd)
	: m_fd(fd) {}

FileDescriptor::~FileDescriptor()
{
	close(m_fd);
}

FileDescriptor::operator int() const
{
	return m_fd;
}

int FileDescriptor::get()
{
	return m_fd;
}

FileDescriptor& FileDescriptor::operator=(int fd)
{
	m_fd = fd;
	return *this;
}

} // namespace ilrd

