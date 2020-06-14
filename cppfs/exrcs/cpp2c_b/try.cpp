#include <iostream>
using namespace std;

class Base
{
public:
	std::size_t m_base;	
};

class Derived : public Base
{
public:
	std::size_t m_derived;
	std::size_t m_derived2;
	std::size_t m_derived3;
};


int main() 
{
	Base* b = new Base();
	b->m_base = 4;
	Derived* d = static_cast<Derived*>(b);
	d->m_base = 2;
	d->m_derived3 = 3;
	
	std::cout << "sizes: " << sizeof(Base) << " " << sizeof(Derived) << std::endl;
	
	std::cout << d->m_base << std::endl;
	std::cout << d->m_derived;
	
	return 0;
}
