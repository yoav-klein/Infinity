/******************************************************
*		int to string and the opposite
*		Author: Yoav Klein
*		Reviewer: Mishel Liberman
*		Date: 26.5.19
********************************************************

#include "itoa.h"

char *IntToString(int base, int num, char *str)
{
	int copy = num;
	int i = 0;
	int length = 0;
	int current = 0;
	
	while(copy > 0)
	{
		copy = copy / base;
		++length;
	}
	
	copy = num;
	
	*(str + length) = EOS;
	for(i = length - 1; i >= 0; --i)
	{
		current = copy % base;
		
		if(current < 10)
		{
			*(str + i) = current + ASCI_DIGIT;
		}
		else
		{
			*(str + i) = current + ASCI_LETTER;
		}
		
		copy /= base;
	}
	
	return str;
}

int StringToInt(int base, char *str)
{
	char *run = str;
	
	int sum = 0, len = 0, i = 0;
	
	int base_asci = 0;
	int curr_base = 1;
	
	if(base < 10)
	{
		base_asci = base + ASCI_DIGIT;
	}
	else
	{
		base_asci = base + ASCI_LETTER;
	}
	
	/* checking until which byte the input is valid */
	while(*run < base_asci && *run != EOS && 
		(*run > 47 && *run < 58) || (*run > 96 && *run < 123))
	{
		++len;
		++run;
	}
	--run;
	
	for(i = 0; i < len; ++i)
	{
		if(*run < 58)
		{
			sum = sum + (*run - ASCI_DIGIT) * curr_base;
		}
		else
		{
			sum = (sum + (*run - ASCI_LETTER)) * curr_base;
		}
		curr_base = curr_base * base;
		--run;
	}
	
	return sum;
}


