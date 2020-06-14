#include <iostream>
using namespace std;

template<typename T>
concept bool MustBeEqual()

void Foo(T t1)
{
	std::cout << t << "Generic Foo" << std::endl;
}

template<>
void Foo<int>(int a)
{
	std::cout << a << "Specialized Foo" << std::endl;
}

int main() 
{
	Foo(5);
	Foo<int>(4);
	
	return 0;
}
