#include <stdio.h> /* printf */
#include <errno.h>  /* perror */
#include <stdlib.h> /* malloc */

#include "../include/cbuf.h"

#define RUN_TEST(x) if(x() == 1) { printf(#x " Success!\n"); }\
					else { printf(#x " Failed\n"); }
					

int Test1()
{
	int test_var_int = 5;
	size_t test_var_long = 10;
	void *buffer = malloc(10);
	int flag = 1;
	cbuf_t *cbuf = CBUFCreate(10);
	int result = 0;
	
	/*Check the function IsEmpty, FreeSpace, Capacity */
	
	result = CBUFIsEmpty(cbuf);
	if(1 != result)
	{
		flag = 0;
	}
	
	result = CBUFFreeSpace(cbuf);
	if(10 != result)
	{
		flag = 0;
	}
	
	result = CBUFCapacity(cbuf);
	if(10 != result)
	{
		flag = 0;
	}
	
	result = CBUFRead(cbuf, buffer, 1); /* Check reading from empty cbuf */
	perror("");
	if(0 != result)
	{
		flag = 0;
	}
	
	if(errno != ENODATA)
	{
		flag = 0;
	}
	
	errno = 0; /* zeroing errno */
	
	/* Test writing 4 bytes from int */
	result = CBUFWrite(cbuf, &test_var_int, 4);
	if(4 != result)
	{
		flag = 0;
	}
	
	/* Test FreeSpace = 6 */
	if(6 != CBUFFreeSpace(cbuf))
	{
		flag = 0;
	}
	
	/* Test IsEmpty */
	result = CBUFIsEmpty(cbuf);
	if(0 != result)
	{
		flag = 0;
	}
	
	/* Tests reading from cbuf */
	result = CBUFRead(cbuf, buffer, 4);
	if(4 != result)
	{
		flag = 0;
	}
	
	/* Free space : 10 */
	if(10 != CBUFFreeSpace(cbuf))
	{
		flag = 0;
	}
	
	/* Test IsEmpty again */
	if(1 != CBUFIsEmpty(cbuf))
	{
		flag = 0;
	}
	
	if(test_var_int != *(int*)buffer) 
	{
		flag = 0;
	}
	
	/* Same test with long, wraping around the end of cbuf */
	result = CBUFWrite(cbuf, &test_var_long, 8);
	if(result != 8)
	{
		flag = 0;
	}
	
	/* Free space: 2 */
	if(2 != CBUFFreeSpace(cbuf))
	{
		flag = 0;
	}
	
	/* Read */
	result = CBUFRead(cbuf, buffer, 8);
	if(8 != result)
	{
		flag = 0;
	}
	
	/* Free space: 10 */
	if(10 != CBUFFreeSpace(cbuf))
	{
		flag = 0;
	}
	
	if(test_var_long != *(size_t*)buffer)
	{
		flag = 0;
	}
	
	/* Overflow the cbuf */
	result = CBUFWrite(cbuf, &test_var_int, 4);
	
	/* Free space: 6 */
	if(6 != CBUFFreeSpace(cbuf))
	{
		flag = 0;
	} 
	
	test_var_long = 0xff;
	result = CBUFWrite(cbuf, &test_var_long, 8);
	
	perror("");
	if(errno != EOVERFLOW)
	{
		flag = 0;
	}
	
	if(result != 6)
	{
		flag = 0;
	}
	
	/*  Checking if there wasn't an override  */
	CBUFRead(cbuf, buffer, 4);
	if(test_var_int != *(int*)buffer)
	{
		flag = 0;
	}
	
	/* Checking if the 6 bytes from the 8 of the long was written properly */
	CBUFRead(cbuf, buffer, 1);
	
	if(0xff != *(unsigned char*)buffer)
	{
		flag = 0;
	}
	
	free(buffer);
	CBUFDestroy(cbuf);
	
	return flag;
}
					
int main()
{

	RUN_TEST(Test1);

	return 1;
}
