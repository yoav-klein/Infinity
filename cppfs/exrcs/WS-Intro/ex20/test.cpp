#include <iostream>
#include <stdio.h>

class X
{
public:
	explicit X(int a);
	void Foo();
	int GetVal() const;
	
private:
	int m_a;
};

X::X(int a)
	: m_a(a)
{
}

int X::GetVal() const
{
	return m_a;
}

void X::Foo()
{
	printf("%d\n", m_a);
}

void Foo(const X& x1)
{
	
	printf("%d\n", x1.GetVal());
}

int main() 
{
	X x1(3);
	x1.Foo();
	//printf("%d\n", x1.m_a);
	Foo(x1);			
	
	return 0;
}
