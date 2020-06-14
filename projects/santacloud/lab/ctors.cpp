#include <iostream>

class X
{
public: 
	X() { std::cout << "Ctor" << this << std::endl; }
	X(const X& other) { std::cout << "CCtor " << &other << " " << this << std::endl; } 
	X& operator=(const X& other) { std::cout << "AOp" << &other << " " << this << std::endl; }
	
private:
	std::size_t m_a;
	std::size_t m_b;
	std::size_t m_c;
};

X foo()
{

	return X();
}

int main()
{
	std::cout << "here" << std::endl;
	X x;
	x = foo();
	
	return 0;
}
