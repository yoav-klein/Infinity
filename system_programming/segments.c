#include <stdio.h>
#include <stdlib.h>

int g_6 = 1;
int g_7 = 2;
int g_8 = 4;
int g = 1;
int g_2;
int g_3;
int g_4;
int g_5;

void Func()
{
	int a;
	
	printf("%p\n", &a);
	
	Func();
}

int main()
{
	

	
	malloc_stats();
	
	return 1;
}	
