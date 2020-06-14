#include <iostream>
#include <cstdio>
class X;



class X
{
public:
	X(int a) : m_a(a) { }
	
	bool operator==(const X& o) const
	{
		return m_a == o.m_a;
	}
	
	int GetValue() const
	{
		return m_a;
	}
	
	X operator+(int a)
	{
		return a + m_a;
	}
	
private:
	int m_a;

};


int operator+(const X& x1, const X& x2)
{
	return x1.GetValue() + x2.GetValue();
}
int main() 
{
	X x1(3);
	X x2(6);
	
	printf("x1 == x2: %d\n", (x1==x2));
	printf("2 + x1: %d\n", 7 + x1);
	printf("2 + x1: %d\n", x1 + 2);
		
	return 0;
}
