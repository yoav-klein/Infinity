/********************************************************
*	Checking which endian the system is
*	Author: Yoav Klein
*	Reviewer: Mishel Liberman
*	Date: 26.5.19
***********************************************************/

#include <stdio.h> /* printf */ 
#define IS_LITTLE_ENDIAN *(int *)"\x00\x00\x00\x01" == 1 ? 0 : 1 


int IsLittleEndian()
{
	int a = 1;
	
	if(1 == *(char*)&a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

int main()
{
	if(1 == IS_LITTLE_ENDIAN)
	{
		printf("The system is little endian\n");	
	}
	else
	{
		printf("The system is big endian\n");
	}
	
	if(1 == IsLittleEndian())
	{
		printf("The system is little endian\n");	
	}
	else
	{
		printf("The system is big endian\n");
	}
	
	
	return 1;
}
