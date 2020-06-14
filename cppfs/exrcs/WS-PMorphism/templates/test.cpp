#include <iostream>

template<typename T>
T foo(T t)
{
	
	return t;
}

template<typename T, int size>
T bar(T t)
{
	int a = size;
	std::cout << a << std::endl;
	
	return t;
}

int main()
{
	foo(4);
	foo(1.3);
	
	bar<int, 3>(3);
	
	return 0;
}
