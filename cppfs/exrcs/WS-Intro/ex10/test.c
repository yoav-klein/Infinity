#include <stdio.h>


int main()
{
	enum E1 { AA, BB, CC, DD };
	enum E2 { TT, YY };
	int i;	
	enum E1 e = 2;
	enum E1 d;
	
	printf("%d\n", d);
	
	e = 3;
	e= TT;
	++e;
	
	i = e;
	return 0;
}
