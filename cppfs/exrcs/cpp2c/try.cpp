#include <iostream>

class Member
{
public:
	Member() { }
};

class Base
{
public:
	virtual void Foo();
	Member m;
	int m_a;
};

void Base::Foo()
{

}

class Derived : public Base
{
public:
	virtual void Foo();
	Member m;
};

void Derived::Foo()
{

}

int main() 
{
	Base *d = new Derived();
	
	return 0;
}
