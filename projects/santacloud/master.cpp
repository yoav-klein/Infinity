/**************************************************************
*	Master
	
	The final master main().
	
	Author: Yoav Klein
	Date: 27.12.19

**************************************************************/

#include <iostream> // std::cout
#include <signal.h> // sigaction
#include <unistd.h> // read
#include <fstream> // std::ofstream
#include <libconfig.h++> // Config

#include "nbd_driver.hpp" // NBDDriver
#include "logger.hpp" // Logger
#include "master_tasks.hpp" // SendRequestTask
#include "nbd_gate.hpp" // NBDGate
#include "request_engine.hpp" // RequestEngine
#include "do_nothing_master.hpp" // ret_do_nothing . function for Factory that returns a 
						   // task that does nothing
#include "stdin_gate_master.hpp" // StdinGate
#include "slave_gate.hpp" // SlaveGate
#include "kamikaze.hpp" // Kamikaze
#include "config_data.hpp" // ConfigDataMaster
#include "exceptions.hpp"
#include "tools.hpp" 

using namespace ilrd;

ProxyDriver* g_proxy_driver;

void sighandler(int sig)
{
	(void)sig;
	
	g_proxy_driver->disconnect();	
}

void set_sigaction(int signal, struct sigaction* act)
{
	if(-1 == sigaction(signal, act, NULL))
	{
		perror("sigaction"); // exception ?
		exit(1);
	}
}

void set_sighandler()
{
	struct sigaction act;
	act.sa_handler = sighandler;
	act.sa_flags = SA_RESTART;
	if (sigemptyset(&act.sa_mask) != 0 ||
    sigaddset(&act.sa_mask, SIGINT) != 0 ||
    sigaddset(&act.sa_mask, SIGTERM) != 0) 
    {
    	perror("sigemptyset"); // exception ?
    	exit(1); 
    }
    
    set_sigaction(SIGINT, &act);
    set_sigaction(SIGTERM, &act);
}

void LOG(Logger::Severity sever, const std::string& msg, int line)
{
	Logger* logger = Handleton<Logger>::get_instance();
	logger->write(sever, std::string(YELLOW "main" RESET), msg, line);
}

ConfigDataMaster* set_configuration()
{
	ConfigDataMaster* conf_data = Handleton<ConfigDataMaster>::get_instance();
	
	libconfig::Config cfg;
	try
	{
		cfg.readFile("master.cfg");
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
		conf_data->m_max_req_size = max_req_kb << 10; // convert to bytes
		conf_data->m_req_timeout = cfg.lookup("req_timeout");
		const libconfig::Setting& root = cfg.getRoot();
		const libconfig::Setting& ports = root["port_list"];
		
		for(int i = 0; i < ports.getLength(); ++i)
		{
			conf_data->m_ports.push_back(ports[i]);
		}
		
  	}
  	catch(const libconfig::SettingNotFoundException &nfex)
  	{
    	LOG(Logger::ERROR, "couldn't find setting in configuration file", __LINE__);
    	throw nfex;
  	}
  	
  	return conf_data;
}


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::cout << "Enter device name" << std::endl;
		exit(1);
	}
	
	// init logger
	Logger* logger = get_logger(); // in libglob.so
	std::ofstream log_file("logfile.txt");
	logger->add_output(log_file);
	DEBUG_ONLY(logger->set_output_severity(Logger::DEBUG));
	
	// libconfig initialization
	ConfigDataMaster* conf_data;
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
	
	std::string dev_name = argv[1];
	
	int num_slaves = conf_data->m_ports.size();
	std::size_t size = conf_data->m_slave_mem_size * num_slaves;  
	// bytes in slave * num_slaves
	set_sighandler();
	
	// Kamikaze 
	std::unique_ptr<Kamikaze> kamikaze;
	try
	{
		kamikaze.reset(new Kamikaze());
	}
	catch(DiskError& err)
	{
		LOG(Logger::ERROR, std::string(RED) + err.what(), __LINE__);
		exit(1);
	}
	
	// NBDDriver
	std::unique_ptr<ProxyDriver> nbd_driver;
	try
	{
		nbd_driver.reset(new NBDDriverProxy(dev_name, size));
	}
	catch(DriverError& err)
	{
		LOG(Logger::ERROR, std::string(RED) + err.who() + err.what(), __LINE__);
		exit(1);
	}
	
	// global for sighandler
	g_proxy_driver = nbd_driver.get();
	
	RequestEngine<int, MasterArgs> req_eng(conf_data->m_dir_path,
										   conf_data->m_num_threads);
	
	// GateWays
	NBDGate nbd_gate(nbd_driver.get(), kamikaze.get());
	StdinGate stdin_gate(nbd_driver.get());
	req_eng.add_gateway(&nbd_gate);
	req_eng.add_gateway(&stdin_gate);
	
	// gateway for each slave
	std::vector<std::unique_ptr<SlaveGate> > slave_gates;
	for(int i = 0; i < num_slaves; ++i)
	{	
		int curr_slave = kamikaze->get_socket_fd(i);
		std::unique_ptr<SlaveGate> slave_gate
		(new SlaveGate(curr_slave, nbd_driver.get(), kamikaze.get()));
		
		req_eng.add_gateway(slave_gate.get());
		slave_gates.push_back(std::move(slave_gate));
	}
	
	// Request related tasks
	req_eng.add_task(0, master_create<SendRequestTask>);
	req_eng.add_task(1, master_create<SendReplyTask>);
	req_eng.add_task(DriverData::DISCONNECT, master_create<DiscTask>);
	req_eng.add_task(DriverData::FLUSH, master_create<OtherTask>);
	req_eng.add_task(DriverData::TRIM, master_create<OtherTask>);
	
	// stdin Tasks
	req_eng.add_task(STOP_KEY, call_disconnect);
	req_eng.add_task(DO_NOTHING_KEY, ret_do_nothing);
	
	try
	{
		req_eng.run();
	}
	catch(std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	
	logger->write(Logger::DEBUG, YELLOW "main" RESET, "main exits", __LINE__);
	
	return 0;
}


/***********************************************************

	Documentation
	
**********************************************************

	Master module - flow description

	this module is responsible for receiving requests from NBD 
	and sending it to slaves using the Kamikaze module.
	
	it works as follows:
	there is NBDGate which holds the socket fd of nbd. when nbd sends
	a request, NBDGate::read() is called, which calls NBDDriver::receive_request(),
	which returns a DriverData holding the data of the request.
	a task of SendRequestTask is created, which calls Kamikaze::send_request(),
	with the DriverData of the request. Kamikaze::send_request() processes the 
	information and sends a request to the relevant slave.
	
	there's also SlaveGate(s), each holding a file descriptor of a slave socket.
	whenever this slave sends a reply, SlaveGate::read() is calles, which calls
	Kamikaze::receive_reply. this function reads the reply from the slave, and
	returns a DriverData holding the data of the request that was replied.
	a task of SendReply is then created, which calls NBDDriver::send_reply().


*************************************************************/

