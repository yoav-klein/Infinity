#include <stdio.h>

extern int g_a;

void Foo()
{
	printf("Foo! g_a: %d, %p\n", g_a, &g_a);
}
