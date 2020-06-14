#ifndef __DRIVER_DATA_HPP__
#define __DRIVER_DATA_HPP__

/**************************************************
	DriverData
	
	this class is used to carry the data of requests from
	the master machine to the slave.
	it's also the base for NBDData.
	
	Author: Yoav Klein
	Date: 8.12.19
 
******************************************************/

#include <string> // std:string
#include <vector> //std::vector

namespace ilrd
{



class DriverData
{
public:

    enum action_type 
    {
        READ = 0,
        WRITE,
        DISCONNECT,
        FLUSH,
        TRIM
    };

    DriverData(action_type type, 
               size_t offset,
               unsigned int length, int req_id = 0);
    DriverData(const DriverData& other) = default;
    DriverData& operator=(const DriverData& other) = default;
    DriverData(DriverData&& other) = default;                  
    DriverData& operator=(DriverData&& other) = default; 
    ~DriverData() = default; 

    action_type get_type() const;
  
    size_t get_offset() const;
    unsigned int get_length() const;
    void set_req_id(int id);
    int get_req_id();
    
    int get_status() const;
    void set_status(int status);

    std::vector<char>& access_buffer(); //read and write - should be separated?

private:
    action_type m_type;
    size_t m_offset;
    unsigned int m_length;
  	int m_req_id;
  	int m_status;
    std::vector<char> m_buffer;
};

}   //namespace ilrd

#endif // __DRIVER_DATA_HPP__



