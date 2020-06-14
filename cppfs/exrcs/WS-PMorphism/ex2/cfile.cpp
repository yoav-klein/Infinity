#include <iostream>
#include "try.h"

template <typename T>
void X<T>::Foo(T t1)
{
	std::cout << t1 << "I'm foo" << std::endl;
}

//template class X<int>;
