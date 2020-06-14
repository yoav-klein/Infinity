
#include <iostream>
#include "handleton.hpp"
#include "dog.hpp"

using namespace ilrd;

extern "C"
Dog* foo()
{
	std::cout << "from foo: " << std::endl;
	
	Dog* dog =  Handleton<Dog>::get_instance();
	
	return dog;
}
