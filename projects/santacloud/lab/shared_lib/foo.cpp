#include <iostream>
#include <memory>
#include "symbols.hpp"

#include "handleton.hpp"
#include "factory.hpp"

using namespace ilrd;

std::unique_ptr<SomeClass> create()
{
	std::cout << "foocreate" << std::endl;
	return std::unique_ptr<SomeClass>();
}


void __attribute__((constructor)) init()
{
	Factory<SomeClass, int>* sc = Handleton<Factory<SomeClass, int> >::get_instance();
	sc->add(2, create);
}

void __attribute__((destructor)) fini()
{
	
	std::cout << "libfoo close" << std::endl;
}
