#include <stdio.h>

int g_a = 7;

void Bla()
{
	
	g_a = 2;
	printf("g_a from bla %p\n", &g_a);
}
