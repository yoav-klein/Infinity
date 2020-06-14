/*********************************************
	MmapStorage
	
	this is a derived class from Storage, that 
	stores the data on a file using mmap.
	used in slave.
	
	Author: Yoav Klein
	Date: 8.12.19

***********************************************/

#ifndef __MMAP_STORAGE_HPP__
#define __MMAP_STORAGE_HPP__


#include "logger.hpp" // Logger
#include "storage.hpp" // Storage
#include "file_raii.hpp" // FileDescriptor


namespace ilrd
{

class MmapStorage : public Storage
{
public:
	MmapStorage(const std::string& file_name, std::size_t size);
	
	std::unique_ptr<DriverData> read(std::unique_ptr<DriverData> data) override;
    std::unique_ptr<DriverData> write(std::unique_ptr<DriverData> data) override;
    
private:
	FileDescriptor m_fd;
	char* m_base;
	Logger* m_logger;
	void LOG(Logger::Severity sever, const std::string& msg, int line);
};



} // ilrd



#endif // __MMAP_STORAGE_HPP__
