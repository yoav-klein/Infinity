#include <stdio.h>
#include "../include/sll.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 
		
sll_t *SLLFlip(sll_t *sll);
			 
int FindNumber(const void *data, const void *key)
{
	if(*(int*)data == (int)(size_t)key)
	{
		return 1;
	}

	return 0;
} 

int Double(void *data, const void *arg)
{
	(void)arg;
	*(int*)data *= 2;
	
	return 0;
}

int BasicFlowTest()
{
	sll_t *sll = SLLCreate();
	it_t iter = NULL;
	
	int arr[5] = { 1, 2, 3, 4, 5 };
	int test_var = 100;
	
	int result = 0;
	int flag = 1;
	int i = 0;
	
	
	/* Check IsEmpty and Size */
	result = SLLIsEmpty(sll);
	if(1 != result)
	{
		flag = 0;
	}
	
	result = (int)SLLSize(sll);
	if(0 != result)
	{
		flag = 0;
	}
	
	for(i = 0; i < 5; ++i) /* Try PushBack  */
	{
		iter = SLLPushBack(sll, &arr[i]);
	}
	
	/* Try IsEmpty and Size */
	result = SLLIsEmpty(sll);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = (int)SLLSize(sll);
	if(5 != result)
	{
		flag = 0;
	}
	
	/* Try Begin, End and Next, and GetData */
	iter = SLLBegin(sll);
	result = *(int*)SLLGetData(iter);
	if(1 != result)
	{
		flag = 0;
	}
	
	iter = SLLNext(iter);
	result = *(int*)SLLGetData(iter);
	if(2 != result)
	{
		flag = 0;
	}
	
	iter = SLLEnd(sll); /* should return the tail, containig deadbeef */
	result = (int)(size_t)SLLGetData(iter); /* cast deadbeef to int  */
	if(result != (int)0xdeadbeef)
	{
		flag = 0;
	}
	
	/* Try PushFront */
	iter = SLLPushFront(sll, &test_var);
	iter = SLLBegin(sll);
	result = *(int*)SLLGetData(iter);
	if(100 != result)
	{
		flag = 0;
	}	
	
	/* Try PopFront */
	SLLPopFront(sll);
	iter = SLLBegin(sll);
	result = *(int*)SLLGetData(iter);
	if(1 != result)
	{
		flag = 0;
	}
	
	/* Now at initial state: 1, 2, 3, 4, 5 */
	/* Try PopEnd */
	
	SLLPopBack(sll);
	SLLPopBack(sll);
	 /* 1, 2, 3 */
	
	iter = SLLNext(SLLBegin(sll));
	iter = SLLNext(iter);
	
	/* Check if we're at the last iterator */
	if(SLLNext(iter) != SLLEnd(sll))
	{
		flag = 0;
	}
	
	/* Check size again */
	if(3 != SLLSize(sll))
	{
		flag = 0;
	}
	
	/* Try InsertAfter */
	iter = SLLBegin(sll);
	SLLInsertAfter(sll, iter, &test_var);
	iter = SLLNext(iter);
	result = *(int*)SLLGetData(iter);
	if(100 != result)
	{
		flag = 0;
	}
	
	/* Try Erase */
	result = (int)SLLSize(sll);
	if(4 != result)
	{
		flag = 0;
	}
	
	SLLErase(sll, iter);
	
	result = (int)SLLSize(sll);
	if(3 != result)
	{
		flag = 0;
	}
	
	iter = SLLBegin(sll);
	iter = SLLNext(iter);
	result = *(int*)SLLGetData(iter);
	if(2 != result)
	{
		flag = 0;
	}
	SLLDestroy(sll);
	
	return flag;
}

int TestFindAndForeach()
{
	sll_t *sll = SLLCreate();
	it_t iter = NULL;
	int arr[5] = { 5, 10, 15, 20, 25 };
	int result = 0;
	int flag = 1;
	int i = 0;
	
	for(i = 0; i < 5; ++i)
	{
		SLLPushBack(sll, &arr[i]);
	}
	
	/* Test SLLFind */
	iter = SLLFind(SLLBegin(sll), SLLEnd(sll), (void*)10, FindNumber);
	result = *(int	*)SLLGetData(iter);
	if(10 != result)
	{
		flag = 0;
	}
	
	iter = SLLFind(SLLBegin(sll), SLLEnd(sll), (void*)29, FindNumber);
	result = (int)(size_t)SLLGetData(iter);
	if((int)0xdeadbeef != result)
	{
		flag = 0;
	}
	
	/* Test foreach */
	
	iter = SLLBegin(sll);
	iter = SLLNext(iter);
	
	result = SLLForEach(iter, SLLEnd(sll), (void*)1, Double);
	
	iter = SLLBegin(sll);
	result = *(int*)SLLGetData(iter);
	if(5 != result)
	{
		flag = 0;
	}
	
	iter = SLLNext(iter);
	result = *(int*)SLLGetData(iter);
	if(20 != result)
	{
		flag = 0;
	}
	
	
	SLLDestroy(sll);
	
	return flag;
}

int TestFlip()
{
	sll_t *sll = SLLCreate();
	it_t iter = NULL;
	
	int arr[] = {1, 2};
	int i = 0;
	int flag = 1;
	
	for(i = 0; i < 2; ++i)
	{
		SLLPushBack(sll, &arr[i]);
	}

	sll = SLLFlip(sll);
	
	iter = SLLBegin(sll);
	
	for(i = 0;  i < 2; ++i)
	{
		printf("%d\n", *(int*)SLLGetData(iter));
		iter = SLLNext(iter);
	}
	
	return flag; 
}



int main()
{
	RUN_TEST(BasicFlowTest);	
	RUN_TEST(TestFindAndForeach);
	RUN_TEST(TestFlip);
	
	return 1;
}


