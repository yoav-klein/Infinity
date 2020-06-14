#include <iostream>

void Unexpected()
{
	std::cout << "UnExpected" << std::endl; 
}

void Foo()
{
	//throw(2);
	//throw((double)2.3);
	throw((float)2.3);	
}

int main()
{
	
	try
	{
		Foo();
	}
	catch(int a)
	{
		std::cout << "Int" << std::endl;
	}
	catch(double d)
	{
		std::cout << "Double" << std::endl;
	}
	/*catch(float f)
	{
		std::cout << "Float" << std::endl;
	}*/
	return 0;
}
