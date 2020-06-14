#include <stdio.h>

#include "../include/dll.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int FindNumber(const void *data, const void *key, const void *params)
{
	(void)params;
	if(data == key)
	{
		return 1;
	}	
	return 0;
}

int Add(void *data, const void *arg)
{
	*(int*)data += *(int*)&arg;
	
	return 0;
}	

void PrintList(dll_t *dll)
{
	it_t i = NULL;
	int count = 0;
	
	for(i = DLLBegin(dll); i != DLLEnd(dll); i = DLLNext(i))
	{
		printf("Node: %d - %d\n", count, *(int*)DLLGetData(i));
		++count; 
	}
	
	printf("\n\n");
}

int TestBasicFlow()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7 ,8 ,9 ,10 };
	int flag = 1;
	int res = 0;
	int test_var = 25;
	int i = 0;
	it_t iter = NULL;
	
	dll_t *dll = DLLCreate();	
	
	res = DLLSize(dll);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = DLLIsEmpty(dll);
	if(1 != res)
	{
		flag = 0;
	}
	
	/* Pushback to list, check returned value, get data */
	for(i = 0; i < 10; ++i)
	{
		iter = DLLPushBack(dll, &arr[i]);
		if(*(int*)DLLGetData(iter) != arr[i])
		{
			flag = 0;
		} 
	}
	
	/* isempty, size */
	res = DLLIsEmpty(dll);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = DLLSize(dll);
	if(10 != res)
	{
		flag = 0;
	}
	
	/* Begin, next, prev, end */
	iter = DLLBegin(dll);
	if(*(int*)DLLGetData(iter) != arr[0])
	{
		flag = 0;
	}
	
	/* Next 3 times */
	iter = DLLNext(iter); /* contains 2 */
	iter = DLLNext(iter); /* contains 3 */
	iter = DLLNext(iter); /* contains 4 */
	
	res = *(int*)DLLGetData(iter);
	if(4 != res)
	{
		flag = 0;
	}
	
	/* add a node */
	iter = DLLInsert(dll, iter, &test_var);
	res = *(int*)DLLGetData(iter);
	if(test_var != res)
	{
		flag = 0;
	}
	
	res = DLLSize(dll);
	if(11 != res)
	{
		flag = 0;
	}
	
	/* Erase, size and check data */
	iter = DLLErase(iter);
	res = DLLSize(dll);
	if(10 != res)
	{
		flag = 0;
	}
	
	res = *(int*)DLLGetData(iter);
	if(5 != res)
	{
		flag = 0;
	}	
	
	/* PopBack, PopFront, getdata, size */
	DLLPopBack(dll);
	DLLPopFront(dll);
	
	res = DLLSize(dll);
	if(8 != res)
	{
		flag = 0;
	}
	
	iter = DLLBegin(dll);
	res = *(int*)DLLGetData(iter);
	if(2 != res)
	{
		flag = 0;
	}
	
	iter = DLLPrev(DLLEnd(dll));
	res = *(int*)DLLGetData(iter);
	if(9 != res)
	{
		flag = 0;
	}
	
	/* PushFront */
	iter = DLLPushFront(dll, &test_var);
	res = DLLSize(dll);
	if(9 != res)
	{
		flag = 0;
	}
	
	iter = DLLBegin(dll);
	res = *(int*)DLLGetData(iter);
	if(res != test_var)
	{
		flag = 0;
	}
	
	DLLDestroy(dll);
	
	return flag;
}

int TestFind()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7 ,8 ,9 ,10 };
	int flag = 1;
	int res = 0;
	int i = 0;
	
	it_t iter = NULL;
	it_t from = NULL;
	it_t to = NULL;
	
	void *data = NULL;
	
	dll_t *dll = DLLCreate();	
	
	/* Pushback to list */
	for(i = 0; i < 10; ++i)
	{
		iter = DLLPushBack(dll, &arr[i]);
	}
	
	from = DLLBegin(dll);
	to = DLLEnd(dll);
	data = (void*)&arr[2];
	
	iter = DLLFind(from, to, FindNumber, NULL, data);
	
	res = *(int*)DLLGetData(iter);
	if(3 != res)
	{
		flag = 0;
	}
	
	/*  Narrow down the range */ 
	
	from = DLLNext(from); /* 2 */
	from = DLLNext(from); /* 3  */
	from = DLLNext(from); /* 4 */
	to = DLLPrev(to); /* 7 */
	to = DLLPrev(to); /* 8 */
	to = DLLPrev(to); /* 9 */
	to = DLLPrev(to); /* 10 */

	iter = DLLFind(from, to, FindNumber, NULL, data);
	
	res = *(int*)DLLGetData(iter);
	if(7 != res)
	{
		flag = 0;
	} 
	
	data = (void*)&arr[3];
	
	iter = DLLFind(from, to, FindNumber, NULL, data);
	
	res = *(int*)DLLGetData(iter);
	if(4 != res)
	{
		flag = 0;
	} 
	
	DLLDestroy(dll);
	
	return flag;
}

int TestForEach()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7 ,8 ,9 ,10 };
	int flag = 1;
	int res = 0;
	int i = 0;
	
	it_t iter = NULL;
	it_t from = NULL;
	it_t to = NULL;
	
	void *data = NULL;
	
	dll_t *dll = DLLCreate();	
	
	/* Pushback to list */
	for(i = 0; i < 10; ++i)
	{
		iter = DLLPushBack(dll, &arr[i]);
		if(*(int*)DLLGetData(iter) != arr[i])
		{
			flag = 0;
		} 
	}
	
	/* Add 10 from 3 to 8  */
	from = DLLBegin(dll);
	from = DLLNext(from);
	from = DLLNext(from); /* 3 */
	
	to = DLLEnd(dll);
	to = DLLPrev(to);
	to = DLLPrev(to);
	
	data = (void*)10;
	
	res = DLLForEach(from, to, Add, data);
	
	/* Check value in from */
	res = *(int*)DLLGetData(from);
	if(13 != res)
	{
		flag = 0;
	}
	
	/*  to is exclusive, should be 9 */
	res = *(int*)DLLGetData(to);
	if(9 != res)
	{
		flag = 0;
	}
	
	to = DLLPrev(to);
	res = *(int*)DLLGetData(to);
	if(18 != res)
	{
		flag = 0;
	}
	
	DLLDestroy(dll);
	
	return flag;
}

int TestSplice()
{
	
	int arr[] = { 1, 2, 3, 4, 5, 6, 7 ,8 ,9 ,10 };
	
	int flag = 1;
	int res = 0;
	int i = 0;
	
	it_t where = NULL;
	it_t from = NULL;
	it_t to = NULL;
	
	dll_t *dll_dest = DLLCreate();	
	dll_t *dll_src = DLLCreate();
	
	/* Pushback to both lists */
	for(i = 0; i < 10; ++i)
	{
		DLLPushBack(dll_dest, &arr[i]);
		DLLPushBack(dll_src, &arr[i]);
	}
	
	where = DLLBegin(dll_dest);
	where = DLLNext(where);
	where = DLLNext(where);  
	where = DLLNext(where); /* Should be appended between 3 - 4 */
	
	from = DLLBegin(dll_src);
	from = DLLNext(from);
	from = DLLNext(from);
	from = DLLNext(from);
	from = DLLNext(from); /* 5 */
	
	to = DLLEnd(dll_src);
	to = DLLPrev(to);
	to = DLLPrev(to); /* 9 */
	
	DLLSpliceBefore(where, from, to);
	
	/*  check sizes */
	res = DLLSize(dll_src);
	if(6 != res)
	{
		flag = 0;
	}	
	
	res = DLLSize(dll_dest);
	if(14 != res)
	{
		flag = 0;
	}
	
	return flag;
	
}

int main()
{
	RUN_TEST(TestBasicFlow);
	RUN_TEST(TestFind);
	RUN_TEST(TestForEach);
	RUN_TEST(TestSplice);
	
	return 1;
}
