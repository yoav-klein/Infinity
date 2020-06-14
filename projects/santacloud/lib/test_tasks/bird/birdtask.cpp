#include <iostream>
#include "handleton.hpp"
#include "factory.hpp"
#include 

using namespace ilrd;

std::unique_ptr<TPTask> create_bird(std::string name, int legs)
{
	std::unique_ptr<TPTask> task(new CatTask(name, legs));
	return task;
}

void __attribute__((constructor)) init()
{
	std::cout << "bird library loaded" << std::endl;
	Factory<TPTask, std::string, std::string, int>* factory = 
	Handleton<Factory<TPTask, std::string, std::string, int> >::get_instance(); 
	
	factory->add("bird", create_bird);
}

void __attribute__((destructor)) fini()
{
	std::cout << "closing catlib" << std::endl;
}


