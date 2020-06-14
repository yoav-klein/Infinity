#include "Exrc2_h.h"

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

void SwapPointers(int **first, int **second)
{
	int *temp;
	temp = *first;
	*first = *second;
	*second = temp;
}
