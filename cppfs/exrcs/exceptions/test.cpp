#include <iostream>

class X
{
public:

	X() { std::cout << "Ctor" << std::endl; }
	~X() { std::cout << "Dtor" << std::endl; } 
};

int main()
{
	X x1;
	
	throw 1;
	
	return 0;
}
