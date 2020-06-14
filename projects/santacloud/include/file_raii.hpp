/*************************************
	RAII object that holds file desciptors 
	and close them upon destruction
**************************************/

#ifndef __FILE_RAII_HPP__
#define __FILE_RAII_HPP__

namespace ilrd
{

class FileDescriptor
{
public:
	FileDescriptor(int fd = 0);
	~FileDescriptor();
	
	operator int() const;
	FileDescriptor& operator=(int fd);
	int get();
	
	FileDescriptor(const FileDescriptor& other) = delete;
	FileDescriptor& operator=(const FileDescriptor& other) = delete;
	FileDescriptor(FileDescriptor&& other) = delete;
	FileDescriptor& operator=(FileDescriptor&& other) = delete;
	
private:
	int m_fd;
};

}
#endif // __FILE_RAII_HPP__
