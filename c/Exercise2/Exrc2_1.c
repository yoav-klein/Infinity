#include "Exrc2_h.h"

size_t Strlen(const char *str)
{
	size_t size=0; /* result */
	const char *runner=str;
	
	while(*runner!='\0')
	{
		++runner;
	}	
	size=runner-str;
	return size;
}	



int Strcmp(const char *s1, const char *s2)
{
	int diff = 0;

	while(*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
	{
		s1++; 
		s2++;
	}
	
	diff = *s1 - *s2;
	return diff;
}

int Strspn(const char *s, const char *accept)
{
	int count = 0;
	int ascii_t[256];
	int i;
	for(i = 0; i < 256; i++)
	{
		ascii_t[i] = 0;
	}
	
	for(; *accept != '\0'; accept++)
	{
		ascii_t[(int)*accept] = 1;
	}
	
	for(; *s != '\0'; s++)
	{
		if(ascii_t[(int)*s] == 0)
		{
			return count;
		}	
		count++;
	}
	return count;
}

int main()
{
char str[] = "	 \t I \tLike Icecream\t ";
ClearSpaces(str);
printf("%sA\n", str);
return 0;

}

