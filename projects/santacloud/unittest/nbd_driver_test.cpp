#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "nbd_driver.hpp"
#include "ram_storage.hpp"
#include "driver_data.hpp"

using namespace ilrd;

NBDDriverProxy* g_nbd_driver;

void sighandler(int sig)
{
	(void)sig;
	
	g_nbd_driver->disconnect();	
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

int main()
{
	set_sighandler();
	
	RAMStorage storage(500000);
	
	try
	{
		NBDDriverProxy nbd_driver("/dev/nbd0", std::size_t(500000));
	
		g_nbd_driver = &nbd_driver;
	
		std::unique_ptr<DriverData> request;
		std::unique_ptr<DriverData> reply;
		while(request = nbd_driver.receive_request())
		{
			switch(request->get_type())
			{
				case DriverData::READ:
				reply = storage.read(std::move(request));
				break;
			
				case DriverData::WRITE:
				reply = storage.write(std::move(request));
				break;
			
				default:
				reply = std::move(request);
				break;
			}
		
			nbd_driver.send_reply(std::move(reply));
		}
	}
	catch(NBDError& error)
	{
		std::cout << error.what() << std::endl;
		std::cout << error.who() << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	

	return 0;
}
