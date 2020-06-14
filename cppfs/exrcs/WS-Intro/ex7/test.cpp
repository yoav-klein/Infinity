#include <iostream>
using namespace std;

int a = 2;
int &a1 = a;

struct X
{
	int a;
	int b;
	int c;
};

void Foo(X &x1)
{
	cout << sizeof(x1);
}

void Bar(int &i)
{
	i = 4;
	cout << i;
}

int main() 
{
	int i = 2;
	
	Bar(i);
	
 	return 0;
}
