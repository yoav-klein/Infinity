#include <iostream>

extern "C"
void Foo()
{
	std::cout << "I'm Foo" << std::endl;
}


void Bar()
{
	std::cout << "I'm Bar" << std::endl;
}
