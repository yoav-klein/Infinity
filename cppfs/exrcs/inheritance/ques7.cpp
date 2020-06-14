#include <iostream>

class X
{
public:
	X() { std::cout << "X Ctor " << this << std::endl; }
	
	//virtual void Foo() { std::cout << "X Foo" << std::endl; }
	//virtual ~X() { std::cout << "X Dtor " << this << std::endl; }
private:
	double m_a;
};

class Y : public X
{
public:
	Y() { std::cout << "Y Ctor" << this << std::endl; }
	//virtual void Foo() {  std::cout << "Y Foo" << std::endl; }
private:
	int m_b;
};


int main()
{
	X *xp = new Y[2];
	
	std::cout << xp << " " << xp + 1 << std::endl;
	std::cout << "X: " << sizeof(X) << "Y: " << sizeof(Y) << std::endl;

	
	delete[] xp;
	
	return 0;
}
