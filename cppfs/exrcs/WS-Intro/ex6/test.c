#include <stdio.h>

struct X 
{
	int a;
};

int main()
{
	const struct X x1 = { 2 };
	
	x1.a = 3;
	
	return 0;
}
