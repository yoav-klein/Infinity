#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

int Memset_Test()
{
	int flag = 1;
	void *test = NULL;
	void *ret = NULL;
	void *exp = calloc(30, sizeof(char));
	
	if(NULL == exp)
	{
		return 0;
	}
	
	test = exp;
	ret = Memset(test, 97, 16);
	if(0 != strcmp("aaaaaaaaaaaaaaaa", (char*)ret))
	{
		flag = 0;
	}
	
	test = (void*)((long)exp + 20);
	ret = Memset(test, 98, 8);
	if(0 != strcmp("bbbbbbbb", (char*)ret))
	{
		flag = 0;
	}
	
	test = (void*)((long)exp + 30);
	ret = Memset(test, 99, 4);
	if(0 != strcmp("cccc", (char*)ret))
	{
		flag = 0;
	}
	
	free(exp);
	return flag;
}

int Memcpy_Test()
{
	int flag = 1;
	char *src = "abcdefghijklmnopqrstuvwxyz";
	char *ret = NULL;
	char *dest = (char*)calloc(40, sizeof(char));
	if(NULL == dest)
	{
		return 0;
	}
	
	ret = Memcpy(dest, src, 16);
	if(0 != strcmp(ret, "abcdefghijklmnop"))
	{
		flag = 0;
	}
	
	ret = Memcpy(dest + 16, src + 4, 2);
	if(0 != strcmp(ret, "ef"))
	{
		flag = 0;
	}
	 
	ret = Memcpy(ret + 2, src + 15, 5);
	if(0 != strcmp(ret, "pqrst"))
	{
		flag = 0;
	}
	
	free(dest);
	return flag;
}

int Memmove_Test()
{	
	int flag = 1;
	char *str = "I love israel";
	char *dest = (char*)calloc(50, sizeof(char));
	
	Memmove(dest, str, strlen(str));
	if(0 != strcmp(dest, "I love israel"))
	{
		flag = 0;
	}
	
	Memmove(dest + 4, dest, 5);
	if(0 != strcmp(dest, "I loI lovrael")) 
	{
		flag = 0;
	}
	
	Memmove(dest, str, strlen(str));
	
	Memmove(dest, dest + 6, 7);
	if(0 != strcmp(dest, " israelisrael"))
	{
		flag = 0;
	} 
	
	free(dest);
	return flag;
}


