#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int IsPalindrome(const char *str)
{
	int size = 0;
	size = strlen(str);
	size--;
	
	printf("----INSIDE IsPalindrom--\n");
	printf("%d\n", size);
	
	for(; str < str + size; str++, size = size - 2)
	{
		printf("%c %c \n", *str, *(str+size));
		if(*str != *(str+size))
			{
			return 1;
			}
	}
	return 0;
}

int main()
{
int res=2;
char *str="AaCCBA";

res = IsPalindrome(str);
printf("%d\n", res);


return 0;
}

