#include <stdio.h>

#pragma weak debug
extern void debug(void);

void (*debugfunc)(void) = debug; 

int main() 
{
	printf("Hello World\n"); 
	if(debugfunc)
	{
		(*debugfunc)();
	}
	
	return 0;
}
