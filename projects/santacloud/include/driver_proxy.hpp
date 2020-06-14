#ifndef __PROXY_DRIVER_HPP__
#define __PROXY_DRIVER_HPP__

/**
 * We are creating a generic driver to manage requests to store and get data.
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
 * This class provides getter and setter for the data.
 * 
 * This class is uncopyable and unmovable.
 * 
 * File:   driver_proxy.hpp
 * Date:   07.11.19
 * Author: Olga Komarovsky Chashka
 * */

#include <string> // std:string
#include <memory> // std::unique_ptr

#include "driver_data.hpp" // DriverData
#include "file_raii.hpp" // FileDescriptor

namespace ilrd
{

class ProxyDriver
{
public:
    ProxyDriver() = default;

    virtual ~ProxyDriver() = 0; 

    virtual std::unique_ptr<DriverData> receive_request() =  0;  
    virtual void send_reply(std::unique_ptr<DriverData> data) = 0;
    virtual void disconnect() = 0;
    int get_request_fd() const; 
    void set_request_fd(int fd);
	
	static void read_all(int fd, char* buf, size_t count);
	static void write_all(int fd, const char* buf, size_t count);
	
	
    ProxyDriver(const ProxyDriver& other) = delete;             
    ProxyDriver& operator=(const ProxyDriver& other) = delete;  
    ProxyDriver(ProxyDriver&& other) = delete;                      
    ProxyDriver& operator=(ProxyDriver&& other) = delete;           
    
private:
    FileDescriptor m_request_fd;
};


}   //namespace ilrd

#endif //__PROXY_DRIVER_HPP__
