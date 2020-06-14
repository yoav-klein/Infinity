#include "Exrc2_h.h"

int Strcasecmp(const char *str1, const char *str2)
{
	char str1_c = 0, str2_c = 0;
	/* next two lines in order to ensure entering the while loop */
	str1_c = tolower(*str1);
	str2_c = tolower(*str2);

	printf("%c %c\n", str1_c, str2_c);
	while ('\0' != str1_c &&'\0' != str2_c && str1_c-str2_c == 0)
	{
		str1_c = tolower(*(++str1));
		str2_c = tolower(*(++str2)); 
		printf("%c %c\n", str1_c, str2_c);
	}
	return (str1_c - str2_c);
}

int Strcasecmp_First(const char *str1, const char *str2)
{
	int i = 0;
	int size_str1 = 0, size_str2 = 0;
	char *str1_new, *str2_new;
	
	size_str1=Strlen(str1);
	size_str2=Strlen(str2);
	
	str1_new=malloc(size_str1);
	str2_new=malloc(size_str2);
	
	Strcpy(str1_new, str1);
	Strcpy(str2_new, str2);
	

	printf("%s %s\n", str1_new, str2_new);
	
	for(i=0; i < size_str1 || i < size_str2; i++)
	{
		if(isupper(*(str1_new+i)) > 0)
		{
		*(str1_new+i) = tolower(*(str1_new+i));
		}
		if(isupper(*(str2_new+i)) > 0)
		{
		*(str2_new+i) = tolower(*(str2_new+i));
	}	}
	printf("%s %s\n", str1_new, str2_new);
	
	return 1;	
}

char *Strstr(const char *big_str, const char *sml_str)
{
	char *ptr_1 = (char*)big_str, *ptr_2 = (char*)sml_str;
	while(*big_str != '\0')
	{
		if(*big_str == *sml_str)
		{
			ptr_1 = (char*)big_str;
			while(*big_str == *sml_str)
			{
				sml_str++;
				big_str++;
				if(*sml_str == '\0')
				{
					return ptr_1;
				}
		}
		sml_str = ptr_2;

		}
		big_str++;
	}
	return NULL;
}
