#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

#include "itoa.h"

int StringToInt_Test()
{
	int flag = 1;
	char *str = (char*)calloc(20, sizeof(char));
	int res = 0;
	
	str = "10a";
	res = StringToInt(16, str);
	if(266 != res)
	{
		flag = 0;	
	}
	
	str = "100";
	res = StringToInt(2, str);
	if(res != 4)
	{
		flag = 0;
	} 
	
	str = "244";
	res = StringToInt(10, str);
	
	if(res != 244)
	{
		flag = 0;
	} 
	
	str = "10104";
	res = StringToInt(2, str);
	if(res != 10)
	{
		flag =  0;
	}
	
	return flag;
}

int IntToString_Test()
{
	int flag = 1;
	char *str = (char*)calloc(20, sizeof(char));
	if(NULL == str)
	{
		printf("Allocation failed\n");
	}
	
	str = IntToString(9, 0, str);
	if(0 != strcmp(str, ""))
	{
		flag = 0;
	}
	
	str = IntToString(10, 200, str);
	if(0 != strcmp(str, "200"))
	{
		flag = 0;
	}
	
	str = IntToString(2, 15, str);
	if(0 != strcmp(str, "1111"))
	{
		flag = 0;
	}
	
	str = IntToString(20, 57, str);
	if(0 != strcmp(str, "2h"))
	{
		flag = 0;
	}
	
	free(str);
	return flag;
}

