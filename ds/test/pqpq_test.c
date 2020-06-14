#include <stdio.h>
#include "../include/pq.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int Priority(const void *data, const void *new_data, const void *params)
{
	(void)params;
	return *(int*)new_data > *(int*)data;
}

int Erase(const void *data, const void *key, const void *params)
{
	(void)params;
	
	return *(int*)data == *(int*)key;
}

int FlowTest()
{
	int flag = 1;
	int result = 0;
	int arr[] = { 1, 5, 10, 9, 4, 100 };
	int i = 0;
	int test_var = 10;
	
	pq_t *pq = PQCreate(Priority);
	
	result = PQSize(pq);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = PQIsEmpty(pq);
	if(1 != result)
	{
		flag = 0;
	}
	
	/* enqueue the arr */
	for(i = 0; i < 6; ++i)
	{
		PQEnqueue(pq, &arr[i]);
	}
	
	/* check size and isempty */
	result = PQIsEmpty(pq);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = PQSize(pq);
	if(6 != result)
	{
		flag = 0;
	}
	
	/* test peek, should be 100  */
	result = *(int*)PQPeek(pq);
	if(100 != result)
	{
		flag = 0;
	}
	
	/* Dequeue, check size and peek */
	PQDequeue(pq);
	
	result = PQSize(pq);
	if(5 != result)
	{
		flag = 0;
	}
	
	result = *(int*)PQPeek(pq);
	if(10 != result)
	{
		flag = 0;
	}
	
	/* clear, check size and isempty */
	PQClear(pq);
	
	result = PQSize(pq);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = PQIsEmpty(pq);
	if(1 != result)
	{
		flag = 0;
	}
	
	/* fill again */
	for(i = 0; i < 6; ++i)
	{
		PQEnqueue(pq, &arr[i]);
	}
	
	/* test erase, check size */
	result = PQErase(pq, &test_var, Erase);
	
	if(0 != result)
	{
		flag = 0;
	}
	
	result = PQSize(pq);
	if(5 != result)
	{
		flag = 0;
	}
	
	/* Dequeue the first */
	PQDequeue(pq);
	
	/* the next should be 9 */
	result = *(int*)PQPeek(pq);
	if(9 != result)
	{
		flag = 0;
	}
	
	test_var = 88;
	/* erase an element that isn't there */
	result = PQErase(pq, &test_var, Erase);
	if(1 != result)
	{
		flag = 0;
	} 
	
	
	PQDestroy(pq);
	
	return flag;
}


int main()
{
	RUN_TEST(FlowTest);
	
	return 1;
}
