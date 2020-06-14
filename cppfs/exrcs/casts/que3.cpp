#include <iostream>

void Foo(const double& d)
{
	std::cout << "Foo: " << d << " " << &d;
	std::cout <<  " " << "Sizeof: " << sizeof(d) << std::endl;
}

void Bar(double& d)
{
	std::cout << "Bar: " << d << " " << &d;
	std::cout << " " << "Sizeof: " << sizeof(d) << std::endl;
}

int main()
{
	int a = 3;
	
	double d = (double)a;
	
	Foo(a);
	Bar(reinterpret_cast<double&>(a));
	
	return 0;
}
