#include <stdio.h>

int Function();

int main()
{	

	unsigned char c = getchar();
	while(c != 'q')
	{
		printf("proc1: %d\n", Function());
		
		c = getchar();
	}
	
	return 0;

}
