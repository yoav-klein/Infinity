#include <iostream>

class X
{
public:
	explicit X();
	operator int();
	//operator unsigned int();
	//operator double();
private:
	int m_a;
};

X::X() : m_a(-7) { }
X::operator int() { std::cout << "To int" << std::endl; return m_a; }
//X::operator double() { return m_a; }
//X::operator unsigned int() { std::cout << "To uint " << m_a << std::endl; return m_a; }

void Fifi(double x)
{
	std::cout << x << std::endl;
}

void Fifi(int x)
{
	std::cout << x << std::endl;
}

int main() 
{
	X x1;
	
	Fifi(-9);
	Fifi(x1);

	return 0;
}
