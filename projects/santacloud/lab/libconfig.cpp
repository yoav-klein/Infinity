#include <libconfig.h++>
#include <iostream>
//#include <string>


void slave()
{
	libconfig::Config cfg;
	
	cfg.readFile("slave.cfg");
	
	try
  	{
  		std::string* name = new std::string();
    	(*name) = cfg.lookup("dir_path").operator std::string();
		std::cout << "dir_path: " << name << std::endl;
  		unsigned int n;
  		n = cfg.lookup("num_threads");
  		//cfg.lookupValue("num_threads", n);
  		std::cout << n;
  		
  	}
  	catch(const libconfig::SettingNotFoundException &nfex)
  	{
    	std::cerr << "No 'name' setting in configuration file." << std::endl;
  	}
}

void master()
{
	libconfig::Config cfg;
	
	cfg.readFile("master.cfg");
	
  	const libconfig::Setting& root = cfg.getRoot();
  	
  	try
  	{
  		const libconfig::Setting& ports = root["ports_num"];
		std::cout <<  ports.getLength() << std::endl;
		for(int i = 0; i < ports.getLength(); ++i)
		{
			int port = ports[i];
			std::cout << port << std::endl;
		}
	}
	catch(const libconfig::SettingNotFoundException &nfex)
    {
    	std::cout << "NOT FOUND" << std::endl;
    }
	
	
}

int main()
{
	
	master();

	return 0;
}
