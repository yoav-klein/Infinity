#include <iostream>
using namespace std;

void Foo()
{
	cout << "Foo void" << endl;
}

void Foo(int i)
{
	cout << "Foo int" << endl;
}

void Foo(char c)
{
	cout << "Foo char" << endl;
}

int main() 
{
	Foo();
	Foo(3);
	Foo('c');
		
	  
	return 0;
}
