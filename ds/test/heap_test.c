#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


struct person 
{
	int id;
	int value;
};

int Compare(const void *data1, const void *data2, const void *params)
{
	(void)params;
	
	return *(int*)data1 < *(int*)data2;
}

int PersonCompare(const void *data1, const void *data2, const void *params)
{
	(void)params;
	
	return ((struct person*)data1)->value < ((struct person*)data2)->value;
}

int ShouldRemove(const void *data, const void *key, const void *params)
{
	(void)params;
	
	return *(int*)data == *(int*)key;
}

int FlowTest()
{
	size_t i = 0;
	int arr[20] = { 12, 50, 120, 3, 500, 45, 16, 85, 22, 13, 15, 65, 87,
					109, 11, 8, 95, 101, 19, 21 };
	
	size_t result = 0;
	int flag = 1;
	int *test = NULL;
	
	heap_t *heap = HeapCreate(Compare);
	
	/* Test size and IsEmpty */
	result = HeapIsEmpty(heap);
	if(1 != result)
	{
		flag = 0;
	}
	
	result = HeapSize(heap);
	if(0 != result)
	{
		flag = 0;
	}
	
	/* Push 20 elements */
	for(i = 0; i < 20; ++i)
	{
		HeapPush(heap, &arr[i]);
	}
	
	result = HeapIsEmpty(heap);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = HeapSize(heap);
	if(20 != result)
	{
		flag = 0;
	}
	
	/* Test Peek */
	test = (int*)HeapPeek(heap); 
	if(500 != *test)
	{
		flag = 0;
	}
	
	/* Remove and check Size and Peek */
	HeapRemove(heap, ShouldRemove, &arr[10]);
	
	result = HeapSize(heap);
	if(19 != result)
	{
		flag = 0;
	}
	
	/* Remove 500 */
	HeapRemove(heap, ShouldRemove, &arr[4]);
	
	result = HeapSize(heap);
	if(18 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(120 != *test)
	{
		flag = 0;
	}
	
	/* Hand-Check using PrintArray. removing 101 */
	HeapRemove(heap, ShouldRemove, &arr[17]);
	
	/* Pop until finishing, check Peek each time */
	
	
	HeapPop(heap);
	
	/* Should be 109 */
	result = HeapSize(heap);
	if(16 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(109 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* should be 95 */
	result = HeapSize(heap);
	if(15 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(95 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 87 */
	result = HeapSize(heap);
	if(14 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(87 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 85 */
	result = HeapSize(heap);
	if(13 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(85 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 65 */
	result = HeapSize(heap);
	if(12 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(65 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 50 */
	result = HeapSize(heap);
	if(11 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(50 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 45 */
	result = HeapSize(heap);
	if(10 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(45 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 22 */
	result = HeapSize(heap);
	if(9 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(22 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 21 */
	result = HeapSize(heap);
	if(8 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(21 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 19 */
	result = HeapSize(heap);
	if(7 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(19 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 16 */
	result = HeapSize(heap);
	if(6 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(16 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 13 */
	result = HeapSize(heap);
	if(5 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(13 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 12 */
	result = HeapSize(heap);
	if(4 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(12 != *test)
	{
		flag = 0;
	}

	HeapPop(heap);
	
	/* Should be 11 */
	result = HeapSize(heap);
	if(3 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(11 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 8 */
	result = HeapSize(heap);
	if(2 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(8 != *test)
	{
		flag = 0;
	}
	
	HeapPop(heap);
	
	/* Should be 3 */
	result = HeapSize(heap);
	if(1 != result)
	{
		flag = 0;
	}
	
	test = (int*)HeapPeek(heap);
	if(3 != *test)
	{
		flag = 0;
	}
	
	/* Last one, check IsEmpty and Size */
	HeapPop(heap);
	
	result = HeapIsEmpty(heap);
	if(1 != result)
	{
		flag = 0;
	}
	
	result = HeapSize(heap);
	if(0 != result)
	{
		flag = 0;
	}
	
	HeapDestroy(heap);

	return flag;
}

int IdenticalValuesTest()
{
	struct person people[10];
	size_t i = 0;
	
	struct person result;
	
	heap_t *heap = HeapCreate(PersonCompare);
	
	/* Push elements */
	for(i = 0; i < 10; ++i)
	{
		people[i].id = i + 1;
		people[i].value = 20;
		
		HeapPush(heap, &people[i]);
	}
	
	/* check the order by popping one by one */
	for(i = 0; i < 10; ++i)
	{
		result = *(struct person*)HeapPeek(heap);
		HeapPop(heap);
		printf("Peek: %d\n", result.id);	
	}
	
	
	HeapDestroy(heap);
	
	return 1;
}

int main()
{
	RUN_TEST(FlowTest);
	/*RUN_TEST(IdenticalValuesTest);*/
	
	return 0;		
}



