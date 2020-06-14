#include "Exrc2_h.h"

char *Strdup(const char *str)
{
	int size = Strlen(str);
	char *new_str = (char*)malloc(size * sizeof(char));
	Strcpy(new_str, str);
	
	return new_str;
}

char *Strcat(char *dest, const char *src)
{
	char *dest_runner;
	dest_runner = dest + Strlen(dest);
	Strcpy(dest_runner, src);

	return dest;
}

char *Strncat(char *dest, const char *src, size_t n)
{
	char *dest_runner;
	dest_runner = dest + Strlen(dest);
	
	Strncpy(dest_runner, src, n);
	
	return dest;
}

void ClearSpaces(char *str)
{
	char *assign = str;
	int flag = 0;
	while('\0' != *str)
	{
		if(*str == ' ' || *str == '\t')
		{	
			if(1 == flag)
			{	
				*assign = *str;
				++str;
				++assign;
				flag = 0;
	
			}
			else
			{
				str++;
			}
		}
		else
		{
			flag = 1;
			*assign = *str;
			assign++;
			str++;
		}
	
	}
	
	if(' ' == *(assign - 1) || '\t' == *(assign-1))
	{
		*(assign - 1) = '\0';
	}
	else
	{
		*assign = '\0';
	}
	
	
}


