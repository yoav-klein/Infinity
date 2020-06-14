#include <iostream>
#include "handleton.hpp"
#include "dogtask.hpp"
#include "factory.hpp"

class SomeClass
{

};

//simulate
/*std::unique_ptr<SomeClass> create()
{
	std::unique_ptr<SomeClass> task(new SomeClass());
	return task;
}*/


namespace ilrd
{

std::unique_ptr<TPTask> create_dog(std::string name, int legs)
{
	std::unique_ptr<TPTask> task(new DogTask(name, legs));
	return task;
}


DogTask::DogTask(std::string name, int legs)
	: TPTask(TPTask::Priority::MEDIUM), m_name(std::move(name)), m_legs(legs) { }

void DogTask::execute()
{
	std::cout << "i'm a dog named: " << m_name << "and i have " << m_legs << 
	"legs!" << std::endl;
}

}  // ilrd

using namespace ilrd;

void __attribute__((constructor)) init()
{

	std::cout << "dog library loaded" << std::endl;
	
	Factory<TPTask, std::string, std::string, int>* factory = 
	Handleton<Factory<TPTask, std::string, std::string, int> >::get_instance(); 
	
	std::cout << "factory instance: " << factory << std::endl;
	factory->add("dog", create_dog);


}


void __attribute__((destructor)) fini()
{
	std::cout << "closing doglib" << std::endl;
}


