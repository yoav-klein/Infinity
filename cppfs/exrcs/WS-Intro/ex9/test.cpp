#include <stdio.h>
#include <iostream>

using namespace std;

struct X
{
	void Foo();
	void Foo() const;
};

void X::Foo()
{
	printf("Foo\n");	
}

void X::Foo() const
{
	printf("const Foo\n");
}

void Function(int a)
{
	cout << "Function" << endl;
}

int main() 
{
	X x1;
	const X x2;
	const X &x3 = x1;
	
	x1.Foo();
	x2.Foo();
	x3.Foo();
	
	return 0;
}
