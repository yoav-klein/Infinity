#include <iostream>

void Foo(const double& d)
{
	std::cout << "Foo: " << d << " " << &d << std::endl;
}

void Bar(double& d)
{
	std::cout << "Bar: " << d << " " << &d << std::endl;
}


int main() 
{
	int a = 3;
	std::cout << "main: " << a << " " << &a << std::endl;
	
	std::cout << reinterpret_cast<double&>(a) << std::endl;
	
	//Foo(a);
	
	Bar(reinterpret_cast<double&>(a));			
	
	return 0;
}
