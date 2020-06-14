#include <iostream>

class X
{
public:
	virtual ~X() { }
	
	void SayHello()
	{ 
		std::cout << "X Hello" << std::endl; 
	}
	
private:
	double m_a;
};

class Y : public X
{
public:
	void SayHello() { std::cout << "Y Hello" << std::endl; };

private:
	int m_b;
};

void Foo(X* x)
{
	std::cout << "Using pointer: " << std::endl;
	x->SayHello();
}

void Foo(X x)
{
	std::cout << "Using copy" << std::endl;
	x.SayHello();
}

int main()
{
	Y y1;
	Foo(y1);
	Foo(&y1);

	return 0;
}
