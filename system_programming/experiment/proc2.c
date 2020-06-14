#include <stdio.h>

int Function();

int main(int argc, char **argv)
{
	unsigned char c = getchar();
	
	printf("Here %s\n", argv[0]);
	
	while(c != 'q')
	{
		printf("proc2: %d\n", Function());
		
		c = getchar();
	}
	c = 1;
	while(c != 'q')
	{
		printf("proc2 - b: %d\n", Function());
		
		c = getchar();
	}
	
	return 0;
}
