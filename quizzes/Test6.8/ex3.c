
#include <stdio.h>
#include <string.h>

int IsHere(char *str, int n, char key)
{
	int i = 0;
	
	for(i = 0; i < n; ++i)
	{
		i += (*str == key) * n;
		++str;
	}
	
	return key == *(str - 1);
}

int main()
{
	char *array = "abcdefgh";	
	
	printf("IsHere: %d\n", IsHere(array, strlen(array), 'h')); 

	return 0;
}
