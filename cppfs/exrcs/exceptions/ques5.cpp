#include <iostream>
#include <exception>
#include <typeinfo>

int Foo(int i)
{
	throw std::bad_alloc();
}

void Bar()
{
	throw std::bad_cast();
}

class X
{
public:
	~X()
	{ 	
	try{
		std::cout  <<  Foo(2) << std::endl; 
		}
		catch(...)
		{
			std::cout << "Caught dtor" << std::cout;
		}
	}
};

void Fishi()
{
	X x1;
	
	throw std::bad_cast();
	//Bar();
	
}

int main()
{
	
	try
	{
		Fishi();
	}
	catch(...)
	{
		std::cout << "main Caught" << std::endl;
	}
	std::cout << "After" << std::endl;
	
	return 0;
}
