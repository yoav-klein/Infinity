/******************************************************
	ConfigData
	
	these two structs are used to hold the data in the
	configuration files of the Slaves and Master.
	
	NOTICE: operator unsigned long() of class Setting
	of the libconfig library doesn't work well, so 
	i had to use unsigned int for the mem_size field.
	
	Author: Yoav Klein
	Date: 15.12.19

*******************************************************/



#ifndef __CONFIG_DATA_HPP__
#define __CONFIG_DATA_HPP__

#include <vector>

namespace ilrd
{

struct ConfigDataSlave
{
	std::string m_dir_path;
	int m_num_threads;
	std::size_t m_slave_mem_size; // MB
	std::string m_file_path; // storage file path
 	int m_max_req_size; // KB
	int m_port_num; 
	std::string m_master_addr;
};


struct ConfigDataMaster
{
	std::string m_dir_path;
	int m_num_threads;
	std::size_t m_slave_mem_size; // MB
	int m_max_req_size; // KB
	int m_req_timeout;
	std::vector<int> m_ports;
};
	


} // ilrd


#endif // __CONFIG_DATA_HPP__
