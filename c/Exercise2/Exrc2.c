#include "Exrc2_h.h"



#define RUN_TEST(test) if(!test()) { printf(#test " OK\n"); }

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



int Strlen_check()
{
	int flag=0;
	char *str1="Hello";
	char *str2="";
	char *str3="1";
	char *str4="Hei man	";
	
	if(5!=Strlen(str1))
	{
 		flag=1; 
	}

	if(0!=Strlen(str2))
	{ 
		flag=1; 
	}

	if(1!=Strlen(str3))
	{ 
		flag=1; 
	}

	if(8!=Strlen(str4))
	{ 
		flag=1; 
	}

	return flag;
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

int Strcmp_check()
{
	char *str1="ABC";
	char *str2="Fd";
	char *str4="";
	int flag=0;
	
	if(0!=Strcmp(str1, str1))
	{	
		flag=1; 
	}

	if(5!=Strcmp(str2, str1))
	{	
		flag=1; 
	}

	if (-5!=Strcmp(str1, str2))
	{	
		flag=1; 
	}

	if(-65!=Strcmp(str4, str1))
	{	
		flag=1; 
	}

	return flag;
}

char *Strcpy(char *dest, const char *src)
{
	char *dest_address = dest;
	while(*src!='\0')
	{
		*dest = *src;
		dest++;
		src++;
	}

	*dest=*src; /* the null character has not been copied inside the for loop */

	return dest_address;
}

int Strcpy_check()
{
	int flag = 0;
	char str1[]= "hello";
	char str2[8];
	Strcpy(str2, str1);
	flag=Strcmp(str2, str1);
	return flag;
}

char *Strncpy(char *dest, const char *src, size_t n)
{
	int i=0;
	int n_ = (int)n;
	for(i=0; i<n_ && *src != '\0'; i++)
	{
		*dest=*src;
		src++;
		dest++;
	}		
	
	
	for(; i<n_; i++)
	{
		*dest = '\0';
		dest++;
	}
	
	return dest;
}


int Strncpy_check()
{
	int flag=0, cmp=0;
	char *src = "Honolulu";
	char dest_first[10];
	char dest_second[10];
	
	Strncpy(dest_first, src, 10);
	cmp = Strcmp(src, dest_first);
	if(0 != cmp)
	{
		flag=1;
	}
		
	Strncpy(dest_second, src, 5);
	cmp = Strcmp("Honol", dest_second);
	if(0 != cmp)
	{
		flag = 1;
	}	
	
	return flag;
}

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

int Strcasecmp_check()
{
	int flag = 0;
	char *str1 = "SabaBa";
	char *str2 = "SABAbA";
	char *str3 = "SadAbads";

	if(0 != Strcasecmp(str1, str2))
	{
		flag = 1;
	}

	if(-2 != Strcasecmp(str1, str3))
	{
		flag = 1;
	}

	if(-2 != Strcasecmp(str2, str3))
	{
		flag = 1;
	}

	return flag;
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
	
	
	printf("---INSIDE strcasecmp----\n");
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

char *Strchr(const char *str, char c)
{
	
	while(*str != c && '\0' != *str)
	{
		str++;
	}
	
	if(*str == c)
	{
		return (char*)str;
	}
	return NULL;
}

int Strchr_check()
{
	int flag = 0;
	char *str = "abcde";
	char a = 'a';
	char d = 'd';
	char e = 'e';
	char x = 'x';

	if(str != Strchr(str, a))
	{
		flag = 1;
	}

	if(str + 3 != Strchr(str, d))
	{
		flag = 1;
	}

	if(str + 4 != Strchr(str, e))
	{
		flag = 1;
	}

	if(NULL != Strchr(str, x))
	{
		flag = 1;
	}

	return flag;

}

char *Strdup(const char *str)
{
	int size = Strlen(str);
	char *new_str = (char*)malloc(size * sizeof(char));
	Strcpy(new_str, str);
	
	return new_str;
}

int Strdup_check()
{
	int flag = 0;
	char *str1, *str2;
	str1 = "";
	str2 = Strdup(str1);

	if(0 != Strcmp(str1, str2))
	{
	flag = 1;
	}
	
	free(str2);
	return flag;
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

int Strcat_check()
{
	char *str_dest_1 = (char*)malloc(100 * (sizeof(char)));
	char *str_dest_2 = (char*)malloc(100 * (sizeof(char)));
	char *str_src = "Mexico";
	int flag = 0;

	/* Check Strcat */ 
	Strcpy(str_dest_1, "I'm going to ");
	Strcat(str_dest_1, str_src);
	flag = Strcmp(str_dest_1, "I'm going to Mexico");
	free(str_dest_1);
	
	/* Check Strncat */
	
	Strcpy(str_dest_2, "I'm going to ");
	Strncat(str_dest_2, str_src, 3);
	flag = Strcmp(str_dest_2, "I'm going to Mex");
	free(str_dest_2);

	return flag;
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

int strstr_check()
{
	int flag = 0;
	char *big_str = "I Was in Holland";
	char *sml_str_1 = "Holland";
	char *sml_str_2 = "HollA";
	char *address_of_holland = big_str + 9;
	char *ret_val;

	ret_val = strstr(big_str, sml_str_1);
	if(ret_val != address_of_holland)
	{
		flag = 1;
	}
	
	ret_val = strstr(big_str, sml_str_2);
	if(ret_val != NULL)
	{
		flag = 1;
	}
	
	return flag;

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

int Strspn_check()
{
	int flag = 0;
	int res=0;
	char *big_str = "I Traveled the world";
	char *accept = "IedT r";
	res = Strspn(big_str, accept);
	
	if(5 == res)
	{
		flag = 1;
	}
	
	return flag;
}

void SwapPointers(int **first, int **second)
{
	int *temp;
	temp = *first;
	*first = *second;
	*second = temp;
}

int SwapPointer_check()
{
	int flag = 0;
	int a = 5, b = 7;
	int *ptr_a = &a;
	int *ptr_b = &b;
	
	SwapPointers(&ptr_a, &ptr_b);
	
	if(ptr_a != &b && ptr_b != &a)
	{
		flag = 1;
	}
	
	return flag;
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



int main()
{
char str[] = "	 \t I \tLike Icecream\t ";
ClearSpaces(str);
printf("%sA\n", str);
return 0;

}

