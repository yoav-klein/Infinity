#ifndef __NBD_DRIVE_HPP__
#define __NBD_DRIVE_HPP__

/**
 * We are creating a wrapper for the NBD driver to allow its use
 * in a generic manner for the purpose of storing data.
 * 
 * NBD is used for the purposes of proof-of-concept and to get a working
 * prototype within a shorter time span. 
 * 
 * At a later stage, resources may be diverted to creation of a more efficient 
 * driver.
 * 
 * The constructor receives the path for the NBD device. It additionally 
 * receives parameters to initialize the device's size - total size or
 * number of blocks and block size.
 * 
 * This class provides an implementation of the pure virtual functions 
 * presented int the DriverProxy class. 
 * 
 * The function receive_request creates and returns a std::unique_ptr to a  
 * DriverData object.
 * 
 * The function send_reply receives a std::unique_ptr to a DriverData object.
 * 
 * The disconnect function allows to initiate a disconnect process in a 
 * controlled manner : activated upon a SIG_TERM, keypress 'q', or at end of 
 * programm.
 * 
 * This class is uncopyable and unmovable.
 * 
 *	Update: 8.12.19 - Working with NBDData. old version in vers.
 *
 *
 * */

#include <string> // std:string
#include <thread> // std::thread
#include <mutex> // std::mutex

#include "driver_proxy.hpp" // interface
#include "logger.hpp" // Logger
#include "file_raii.hpp" // FileDescriptor
#include "nbd_data.hpp" // NBDData
#include "exceptions.hpp" // NBDError

namespace ilrd
{


class NBDDriverProxy: public ProxyDriver
{
public:
    NBDDriverProxy(const std::string& dev_file, std::size_t storage_size);
    NBDDriverProxy(const std::string& dev_file, 
                   std::size_t block_size,  
                   std::size_t num_blocks);
    ~NBDDriverProxy() override; 

    std::unique_ptr<DriverData> receive_request() override;  
    void send_reply(std::unique_ptr<DriverData> data) override;
    void disconnect() override; 
    
private:
	void setup(const std::string& dev_file);
	void call_do_it();
	static void read_all(int fd, char* buff, size_t count);
	static void write_all(int fd, char* buff, size_t count);
	
	template<typename T, typename... Types>
	static void call_ioctl(T var1, Types... var2);
	
    FileDescriptor m_nbd_fd; // returned by open()
    FileDescriptor m_nbd_socket;           
    std::thread m_nbd_thread;
    std::mutex m_mutex;
    bool m_is_disconnected;
    Logger* m_logger;
    void LOG(Logger::Severity sever, const std::string& msg, int line);
};

}   //namespace ilrd

#endif// __NBD_DRIVE_HPP__
