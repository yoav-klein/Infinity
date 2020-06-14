#include <stdio.h>

int g_a;

int main()
{
	int a = 2;
	g_a = 3;
	printf("%p\n", &g_a);
	return 0;
}
