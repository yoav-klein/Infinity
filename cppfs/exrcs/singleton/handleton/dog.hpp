#include <iostream>

class Dog
{
public:
	Dog() { }
	~Dog() { std::cout << "dtor" << std::endl; }

};
