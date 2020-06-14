#include <iostream>

class X
{
public:
	explicit X();
	X(int a);
	explicit X(int a, int b);
	X(const X& other);
	
	void Foo();
	
private:
	int m_a;
};

X::X()
	: m_a(0) { }

X::X(int a)
	: m_a(a) { }

X::X(int a, int b)
	: m_a(a + b) { }

X::X(const X& other) { m_a = other.m_a; } 
	
void X::Foo()
{
	std::cout << m_a << std::endl;
}

void Fifi(X x)
{
	x.Foo();
}


int main() 
{
	X x1(7);
	
	Fifi(x1);
	Fifi((X)7); // one way
	Fifi(X(6)); // second way
	Fifi(static_cast<X>(8));
	
	Fifi(9.3);
	 //Fifi(X(3, 4));
		
	return 0;
}
