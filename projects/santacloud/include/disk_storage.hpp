/**********************************************
	DiskStorage class
	Author: Yoav Klein
	Date: 21.11.19
	
	A derived class of Storage. the class writes
	and reads from/to a file which it's path is
	given to the ctor as argument.
	
	reads and writes are thread safe, i.e. protected with mutex.
	
	the file is set to size of 'size' parameter in the ctor.
	could throw a DiskError.
	
	the ctor can also throw if the opening of the file failed.
	
	if read or write operations fail, the status of the request will be 
	set to -1
	
	the user must have a file with a name corresponding to the
	argument given to file_name parameter

**************************************************/

#ifndef __DISC_STORAGE_HPP__
#define __DISC_STORAGE_HPP__

#include <mutex> // std::mutex
#include "logger.hpp" // Logger
#include "storage.hpp" // base class
#include "exceptions.hpp" // DiskError

#include "file_raii.hpp"

namespace ilrd
{

class DiskStorage : public Storage
{
public:
	DiskStorage(std::string file_name, std::size_t size);

 	std::unique_ptr<DriverData> read(std::unique_ptr<DriverData> data) override;
    std::unique_ptr<DriverData> write(std::unique_ptr<DriverData> data) override;

private:
	FileDescriptor m_fd;
	std::mutex m_mutex;
	Logger* m_logger;
};


} // ilrd
#endif // __DISC_STORAGE_HPP__
