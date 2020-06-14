#include <iostream>
#include "handleton.hpp"
#include "cattask.hpp"
#include "factory.hpp"

class SomeClass
{

};

// simulate
/*std::unique_ptr<SomeClass> create()
{
	std::unique_ptr<SomeClass> task(new SomeClass());
	return task;
}*/
namespace ilrd
{

std::unique_ptr<TPTask> create_cat(std::string name, int legs)
{
	std::unique_ptr<TPTask> task(new CatTask(name, legs));
	return task;
}


CatTask::CatTask(std::string name, int legs)
	: TPTask(TPTask::Priority::MEDIUM), m_name(std::move(name)), m_legs(legs) { }

void CatTask::execute()
{
	std::cout << "i'm a cat named: " << m_name << "and i have " << m_legs << 
	"legs!" << std::endl;
}

} // ilrd

using namespace ilrd;

void __attribute__((constructor)) init()
{
	std::cout << "cat library loaded" << std::endl;
	
	Factory<TPTask, std::string, std::string, int>* factory = 
	Handleton<Factory<TPTask, std::string, std::string, int> >::get_instance(); 
	
	std::cout << "factory instance: " << factory << std::endl;
	
	factory->add("cat", create_cat);

}


void __attribute__((destructor)) fini()
{
	std::cout << "closing catlib" << std::endl;
}


