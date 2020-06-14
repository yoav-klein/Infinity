#include <stdio.h>
#include <iostream>


struct X
{
	void Init(int a_);
	void Foo();
	void Bar() const;
	void Boo();
	
	int m_a;
	int *m_p;
};

void X::Init(int a_)
{
	m_a = a_;
	m_p = &m_a;
}

void X::Foo()
{
	++m_a;
	--(*m_p);
}

void X::Bar() const
{
	printf("%d\n", m_a);
	printf("%d\n", *m_p);
	printf("%p\n", m_p);
	
	//m_a = 0;
	// m_p = 0;
	//*m_p = 0;
}

void Fifi(const X& x_)
{
	//x_.Foo();
	x_.Bar();
}

int main()
{
	X x1;
	
	x1.Init(2);
	x1.Foo();
	Fifi(x1);
	
	return 0;
}

