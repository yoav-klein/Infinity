/****************************************************************
*			Letters in two arrays
*			Author: Yoav Klein
*			Reviewer: Mishel Liberman
*			Date: 26.5.19
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASCI_LENGTH 256


void ThreeArrays(char *str1, char *str2, char *str3, int len1, int len2, int len3)
{
	char *asci_lut = (char*)calloc(ASCI_LENGTH, sizeof(char));
	if(NULL == asci_lut)
	{
		perror();
		return;
	}
	int i = 0;
	
	for(i = 0; i < len1; ++i)
	{
		*(asci_lut + *(str1 + i)) = 1;
	}
	
	for(i = 0; i < len3; ++i)
	{
		*(asci_lut + *(str3 + i)) = 0;
	}
	
	for(i = 0; i < len2; ++i)
	{
		if(1 == *(asci_lut + *(str2 + i)))
		{
			*(asci_lut + *(str2 + i)) = 0;
			printf("match: %c\n", *(str2 + i));
		}
	}
	
	free(asci_lut);
}

int main()
{
	char *str1 = "abcdefghijklmnop";
	char *str2 = "qrstuabcdefg";
	char *str3 = "yzcef";
	
	ThreeArrays(str1, str2, str3, strlen(str1), strlen(str2), strlen(str3));
	
	return 1;
}
