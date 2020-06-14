
#include <iostream>
#include <memory>
#include "symbols.hpp"
#include "handleton.hpp"
#include "factory.hpp"

using namespace ilrd;

std::unique_ptr<SomeClass> create()
{
	std::cout << "create" << std::endl;
	return std::unique_ptr<SomeClass>();
}

void __attribute__((constructor)) init()
{
	
	Factory<SomeClass, int>* sc = Handleton<Factory<SomeClass, int> >::get_instance();
	sc->add(1, create);

}

void __attribute__((destructor)) fini()
{
	std::cout << "libbar close" << std::endl;
	//std::cout << fifi<int>()::uptr.get() << std::endl;
}
