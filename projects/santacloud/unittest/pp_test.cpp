#include <iostream>
#include <unistd.h>
#include <dlfcn.h>

#include "dir_monitor.hpp"
#include "handleton.hpp"
#include "dll_loader.hpp"
#include "factory.hpp"
#include "tptask.hpp"
#include "thread_pool.hpp"

class SomeClass
{

};


namespace ilrd
{
	Logger* get_logger();
}

using namespace ilrd;


void catdog()
{
	
	Factory<TPTask, std::string, std::string, int>* factory = 
	Handleton<Factory<TPTask, std::string, std::string, int> >::get_instance(); 
	
	std::cout << "factory in main: " << factory << std::endl;
	
	void* handle_dog = dlopen("/home/student/git/projects/santacloud/lib/libdog.so", 
	RTLD_NOW | RTLD_GLOBAL);
	if(handle_dog == nullptr)
	{
		perror("dlopen");
		exit(1);
	}
	
	void* handle_cat = dlopen("/home/student/git/projects/santacloud/lib/libcat.so", 
	RTLD_NOW | RTLD_GLOBAL);
	
	if( handle_cat == nullptr)
	{
		perror("dlopen");
		exit(1);
	}
	
	
	dlclose(handle_cat);
	dlclose(handle_dog);

}

void foobar()
{
	
	
	void* handle_foo = dlopen("/home/student/git/projects/santacloud/lab/shared_lib/libfoo.so", RTLD_LAZY);
	void* handle_bar = dlopen("/home/student/git/projects/santacloud/lab/shared_lib/libbar.so", RTLD_LAZY);

		
	if(nullptr == handle_bar || nullptr == handle_foo)
	{
		std::cout << "error dlopening" << std::endl;
		exit(1);
	}
	
	
	dlclose(handle_foo);
	dlclose(handle_bar);
}

void debug()
{
	catdog();
	//foobar();
}

void test()
{
	Logger* logger = get_logger();
	logger->set_output_severity(Logger::DEBUG);
	
	logger->write(Logger::DEBUG, "main", "starting program", __LINE__);
	DirMonitor dir_monitor("/home/yoav/git/projects/santacloud/lib");
	DllLoader dll_loader(dir_monitor.get_dispatcher());
	
	//Factory<TPTask, std::string, std::string, int>* factory =
	//Handleton<Factory<TPTask, std::string, std::string, int> >::get_instance();
	
	sleep(20);
	/*std::cout << "factory address in main: " << factory << std::endl;
	//ThreadPool tp;
	
	char c = '0';
	while(c != 'q')
	{
		std::string key;
		std::string name;
		int legs;
		
		std::cout << "enter key" << std::endl;
		std::cin >> key;
		
		std::cout << "enter name" << std::endl;
		std::cin >> name;
		
		std::cout << "num legs?" << std::endl;
		std::cin >> legs;
		
		try
		{
			std::unique_ptr<TPTask> task = factory->create(key, name, legs);
			//tp.add_task(std::move(task));
		}
		catch(BadKey& b)
		{
			std::cout << "no such key" << std::endl;
			std::cout << b.what() << std::endl;
		}
	
	
		std::cout << "press q to quit, other key to continue" << std::endl;
		std::cin >> c;
	}*/

}

int main()
{
	//test();
	
	debug();
	
	std::cout << "end of main" << std::endl;
	return 0;
}
