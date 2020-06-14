

#include <iostream>

class Base
{	
public:
	Base(int a) : m_a(a) 
	{
		std::cout << "B::Ctor" << std::endl;
	}
	 virtual ~Base() 
	{
		std::cout << "B::Dtor" << std::endl;
	}
	
	virtual void Print1() const
	{
		std::cout << "B::Print1 B::m_a - " << m_a << std::endl;
	}
	
	void Print2() const
	{
		std::cout << "B::Print2" << std::endl;
	}
	
	
	virtual void Print3() const
	{
		std::cout << "B::Print3" << std::endl;
	}
	
private:
	int m_a;
};

class Base2
{
	virtual void Foo() { std::cout << "Foo" << std::endl; }
};

class Derived: public Base, public Base2
{
public:
	Derived() : Base(6), m_b(2)
	{
		std::cout << "D::Ctor" << std::endl;
	}
	
	~Derived()
	{
		std::cout << "D::Dtor" << std::endl;
	}
	
	void Print1() const;
	virtual void Print2() const
	{
		std::cout << "D::Print2" << std::endl;
	}
	
private:
	int m_b;
};



void Derived::Print1() const
{
	std::cout << "D::Print1 D::m_b - " << m_b << std::endl;
	Base::Print1();
	std::cout << "D::Print 1 End" << std::endl;
}


int main() 
{
	Base *b1 = new Base(4);
	Base *b2 = new Derived;
	Base2 *b3 = new Derived;
	
	std::cout << "Sizes: " << sizeof(b1) << sizeof(*b2) << std::endl;
	
	std::cout << "main b1:" << std::endl;
	b1->Print1();
	b1->Print2();
	b1->Print3();
	
	std::cout << "main b2:" << std::endl;
	b2->Print1();
	b2->Print2();
	b2->Print3();
	
	Derived *d = static_cast<Derived*>(b2);
	
	std::cout << "main d:" << std::endl;
	d->Print2();
	b2->Print2();
	d->Print3();
	
	delete b2;
	delete b1;
		
	return 0;
}
