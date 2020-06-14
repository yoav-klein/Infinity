#include <iostream>
#include "try.h"

class Y<int>;

/*template<>
void Foo<int>(int a)
{
	std::cout << "Specialized: " << a << std::endl;
}*/

/*
template<typename V>
void Foo(X<V> x1)
{
	std::cout << V;
}*/

int main() 
{
	
  	X<int> x1;
  	x1.t1 = 2;
  	
  	x1.Foo(4);
  	//int a = 4;
  	//Foo(a);
  
	return 0;
}
