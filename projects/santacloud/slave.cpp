/***********************************************	
	Slave main
	
	receives requests from master, and replies.
	
	Author: Yoav Klein
	Date: 1.1.20

****************************************************/


#include <libconfig.h++> // Config
#include <fstream> // std::ofstream

#include "logger.hpp" // Logger
#include "slave_driver.hpp" // SlaveDriver
#include "mmap_storage.hpp" // MmapStorage
#include "ram_storage.hpp" // RAMStorage
#include "udp_gate.hpp" // UDPGate
#include "request_engine.hpp" // RequestEngine
#include "slave_tasks.hpp" // ReadTask
#include "stdin_gate_slave.hpp" // StdinGate
#include "do_nothing_slave.hpp" // DoNothingTasks
#include "config_data.hpp" // ConfigDataSlave
#include "exceptions.hpp"
#include "tools.hpp"

using namespace ilrd;

void LOG(Logger::Severity sever, const std::string& msg, int line)
{
	Logger* logger = Handleton<Logger>::get_instance();
	logger->write(sever, std::string(YELLOW "main" RESET), msg, line);
}

ConfigDataSlave* set_configuration()
{
	ConfigDataSlave* conf_data = Handleton<ConfigDataSlave>::get_instance();
	
	libconfig::Config cfg;
	try
	{
		cfg.readFile("slave.cfg");
	}
	catch(const libconfig::FileIOException &fioex)
  	{
		LOG(Logger::ERROR, "I/O error while reading file.", __LINE__);
   	 	throw fioex;
  	}
  	catch(const libconfig::ParseException &pex)
  	{
        LOG(Logger::ERROR, "Parse error", __LINE__);
    	throw pex;
  	}
  	
  	try
  	{
		conf_data->m_dir_path = cfg.lookup("dir_path").operator std::string();
		conf_data->m_num_threads = cfg.lookup("num_threads");
		int slave_mem_mb = cfg.lookup("slave_mem_size"); // convert to bytes:
		conf_data->m_slave_mem_size = (std::size_t)slave_mem_mb << 20;
		int max_req_kb = cfg.lookup("max_req_size");
		conf_data->m_file_path = cfg.lookup("storage_file_path").operator std::string();
		conf_data->m_max_req_size = max_req_kb << 10; // convert to bytes
		conf_data->m_port_num = cfg.lookup("port_num");
		conf_data->m_master_addr = cfg.lookup("master_addr").operator std::string();
  	}
  	catch(const libconfig::SettingNotFoundException &nfex)
  	{
    	LOG(Logger::ERROR, "couldn't find setting in configuration file", __LINE__);
    	throw nfex;
  	}
  	
  	return conf_data;
}

int main()
{
	// init logger
	Logger* logger = get_logger();
	std::ofstream log_file("logfile.txt");
	logger->add_output(log_file);
	logger->set_output_severity(Logger::DEBUG);
	
	// libconfig initialization
	ConfigDataSlave* conf_data;
	try
	{
		conf_data = set_configuration();
	}
	catch(const libconfig::ConfigException &ex)
    {
		LOG(Logger::ERROR, std::string("configuration setting failed: ") + 
			ex.what(), __LINE__);
		exit(1);
	}
	
	int max_req = conf_data->m_max_req_size;
	std::size_t slave_size = conf_data->m_slave_mem_size;
	std::string file_path = conf_data->m_file_path;
	// explanation for this calculation below
	std::size_t size = (slave_size + max_req) * 2;
	
	// MmapStorage 
	std::unique_ptr<Storage> storage;
	try
	{
		storage.reset(new MmapStorage(file_path, size));
	}
	catch(DiskError& err)
	{
		LOG(Logger::ERROR, std::string(RED) + err.what(), __LINE__);
		exit(1);
	}
	
	// SlaveDriver
	std::unique_ptr<SlaveDriver> slave_driver;
	try
	{
		slave_driver.reset(new SlaveDriver());
	}
	catch(DriverError& err)
	{
		LOG(Logger::ERROR, std::string(RED) + err.who() + err.what(), __LINE__);
		exit(1);
	}
	
	// GateWays
	UDPGate udp_gate(slave_driver.get(), storage.get());
	StdinGate stdin_gate(slave_driver.get());
	
	RequestEngine<int, DataArgs> req_eng(conf_data->m_dir_path, 
										 conf_data->m_num_threads);
	
	req_eng.add_gateway(&stdin_gate);
	req_eng.add_gateway(&udp_gate);
	
	// UDP tasks
	req_eng.add_task(DriverData::READ, slave_create<ReadTask>);
	req_eng.add_task(DriverData::WRITE, slave_create<WriteTask>);
	
	// Stdin Tasks
	req_eng.add_task(STOP_KEY, stop_running);
	req_eng.add_task(DO_NOTHING_KEY, ret_do_nothing);
	
	slave_driver->ping_master();
	try
	{
		req_eng.run();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	LOG(Logger::DEBUG, "main exits", __LINE__);
	
	return 0;
}


/**********************************************************
	
	Documentation

**********************************************************
	
	slave module - serves requests from master.
	
	uses SlaveDriver that holds a socket that's connected 
	to the master. using this socket it receives requests
	and send replies.
	
	MmapStorage is used to store the data, in a file that it's path
	is specified in the configuration file.

	the size of the file is calculated as following:
	size = (slave_size + max_request_size) * 2;
	the reason for the additional max_request_size: let's say a slave size
	is 10000 bytes, (20000 including the backup). 
	it may receive a request to write in offset 9990, a length of max_request_size
	(let's say 4096). in that case, the storage part will overrun the backup part.
	that's why we need a safety buffer of max_request_size right after the storage
	part, so that backup will be written from after that safety buffer.
	another max_request_size is allocated to the end of the backup part, since
	in the scenario described above, if we won't have this extra space, we could
	write to a place not allocated for us and receive a segmentation fault.
	
	flow of slave:
	
	there's a UDPGate gateway, that holds the fd of the socket of master.
	whenever master sends a request, UDPGate::read() is called, which calls
	SlaveDriver::receive_request(), which reads the request from the master
	and returns a DriverData, holding the info of that request.
	a task of WriteTask or ReadTask is then created, writing or reading to\from 
	disk, and replying master. 
	
*******************************************/

