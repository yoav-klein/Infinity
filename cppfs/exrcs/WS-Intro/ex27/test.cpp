#include <iostream>
using namespace std;

class X
{
	friend ostream& operator<<(ostream& os, const X& x);
	
public:
	X(int a) : m_a(a) {  }
	
	int GetValue() const { return m_a; }

private:
	int m_a;	
};

int Foo()
{
 return 0;
}

int main()
{
	int (*func)() = Foo;
	int (X::*gv_ptr)() const; 
	X x1(3);
	X x2(6);
	int i = 78;
	double d = 3.14;
	char c = 'a';
	
	gv_ptr = &X::GetValue;
	
	std::cout << *(void**)(&X::GetValue);
	
	std::cout << "x1: " << x1 << "x2: " << x2 << " " <<
	" " << i << " " << &i << " " << d << " " << &d << " "
	<< c << " " << (void*)&c << std::endl;
	
	return 0;

}
	
ostream& operator<<(ostream& os, const X& x)
{
	return os << x.m_a;
}


