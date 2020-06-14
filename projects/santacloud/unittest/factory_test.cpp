// 
// simulates a factory that it's key is an enum
// 
//

#include <iostream>

#include "factory.hpp"
using namespace ilrd;

class SomeClass
{
public:
	SomeClass(int n)
		: m_n(n) { }
	void show() { std::cout << "i'm " << m_n << std::endl; }
private:
	int m_n;
};

enum nums
{
	ONE,
	TWO,
	THREE
};

template <int n>
std::unique_ptr<SomeClass> creator(int a, int b, int c)
{
	std::cout << "here with: " << a << " " << b << " " << c << std::endl;
	return std::unique_ptr<SomeClass>(new SomeClass(n));
}

int main()
{
	Factory<SomeClass, int, int, int, int> factory;
	
	factory.add(nums::ONE, creator<1>);
	factory.add(2, creator<2>);
	factory.add(3, creator<3>);
	
	std::unique_ptr<SomeClass> ptr = factory.create(1, 1, 2, 3);
	
	ptr->show();
	
	return 0;
}
