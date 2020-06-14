#include <stdio.h>

#include "../include/srtl.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int Add(void *data, const void *arg)
{
	*(int*)data += *(int*)arg;
	
	return 0;
}

int Find(const void *data, const void *key, const void *params)
{
	(void)params;
	return *(int*)data == *(int*)key;
}

void PrintList(srtl_t *list)
{
	sit_t current = SrtLBegin(list);
	
	for(; current != SrtLEnd(list); current = SrtLNext(current))
	{
		printf("%d --> ", *(int*)SrtLGetData(current));
	}
	printf("\n");
}

int Sort(const void *data, const void *key, const void *params)
{
	if(*(int*)&params > 0)
	{
		return *(int*)data > *(int*)key;
	}
	else
	{
		return *(int*)data < *(int*)key;
	}
}


int TestFlow()
{
	int flag = 1;
	int result = 0;
	int arr[] = { 5, 2, 10, 94, 71, -1, 50 };
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *srtl = SrtLCreate(Sort, (void*)1); /* Ascending order */
	
	result = SrtLIsEmpty(srtl);
	if(1 != result)
	{
		flag = 0;
	}
	
	result = SrtLSize(srtl);
	if(0 != result)
	{
		flag = 0;
	}
	
	iter = SrtLInsert(srtl, &arr[0]); /* Insert 5 */
	/* Check size and isempty  */
	result = SrtLIsEmpty(srtl);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = SrtLSize(srtl);
	if(1 != result)
	{
		flag = 0;
	}
	
	for(i = 0; i < 7; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(srtl, &arr[i]);
	}
	

	
	result = SrtLSize(srtl);
	if(8 != result)
	{
		flag = 0;
	}
	
	/* Check if the first is -1  */
	iter = SrtLBegin(srtl);
	result = *(int*)SrtLGetData(iter);
	
	if(-1 != result)
	{
		flag = 0;
	}
	
	/* the next should be 2 */
	iter = SrtLNext(iter);
	result = *(int*)SrtLGetData(iter);
	if(2 != result)
	{
		flag = 0;	
	}
	
	
	/* the next 2 should be 5 */ 
	iter = SrtLNext(iter);
	result = *(int*)SrtLGetData(iter);
	if(5 != result)
	{
		flag = 0;	
	}
	
	iter = SrtLNext(iter);
	result = *(int*)SrtLGetData(iter);
	if(5 != result)
	{
		flag = 0;	
	}
	
	/* PopBack, PopFront, Check size */
	SrtLPopBack(srtl);
	
	SrtLPopFront(srtl);
	
	result = SrtLSize(srtl);
	if(6 != result)
	{
		flag = 0;
	}
	
	/* Check the first and last value */
	iter = SrtLBegin(srtl);
	result = *(int*)SrtLGetData(iter);
	
	if(2 != result)
	{
		flag = 0;
	}
	
	iter = SrtLNext(iter); /*  5 */
	iter = SrtLNext(iter); /* 5 */
	iter = SrtLNext(iter); /* 10 */
	iter = SrtLNext(iter); /* 50 */
	iter = SrtLNext(iter); /* 71 */
	
	result = *(int*)SrtLGetData(iter);
	if(71 != result)
	{
		flag = 0;	
	}
	
	/* check previous */
	iter = SrtLPrev(iter);
	result = *(int*)SrtLGetData(iter);
	if(50 != result)
	{
		flag = 0;	
	}
	
	SrtLDestroy(srtl);
	
	return flag;
}

int TestFind()
{
	int flag = 1;
	int result = 0;
	int arr[] = { 5, 2, 10, 94, 71, -1, 50 };
	sit_t iter = NULL;
	int i = 0;
	int test_var = 71;
	
	srtl_t *srtl = SrtLCreate(Sort, (void*)1); /* Ascending order */
	
	for(i = 0; i < 7; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(srtl, &arr[i]);
	}
	
	/* look for 71 */
	iter = SrtLFind(SrtLBegin(srtl), SrtLEnd(srtl), Find, &test_var, NULL);

	
	result = *(int*)SrtLGetData(iter);
	if(71 != result)
	{
		flag = 0;
	}
	
	/* look for 94 where 71 is to. should return 71 */
	test_var = 71;	
	iter = SrtLFind(SrtLBegin(srtl), iter, Find, &test_var, NULL);
	result = *(int*)SrtLGetData(iter);
	if(71 != result)
	{
		flag = 0;
	}
	
	/* look for 10 where 71 is from should return End */
	test_var = 10;
	iter = SrtLFind(iter, SrtLEnd(srtl), Find, &test_var, NULL);
	
	if(iter != SrtLEnd(srtl))
	{
		flag = 0;
	}
	
	SrtLDestroy(srtl);
	
	return flag;
}

int TestForEach()
{
	int flag = 1;
	int result = 0;
	int arr[] = { 5, 2, 10, 94, 71, -1, 50 };
	sit_t iter = NULL;
	int i = 0;
	int test_var = 10;
	
	srtl_t *srtl = SrtLCreate(Sort, (void*)1); /* Ascending order */
	
	for(i = 0; i < 7; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(srtl, &arr[i]);
	}

	/* add 10 from 3rd element: 5 */
	iter = SrtLBegin(srtl);
	iter = SrtLNext(iter);
	iter = SrtLNext(iter);
	
	result = SrtLForEach(iter, SrtLEnd(srtl), Add, &test_var);
	
	/* GetData, should be 15 */
	result = *(int*)SrtLGetData(iter);
	if(15 != result)
	{
		flag = 0;
	}
	
	/* one before, should be 2 */
	iter = SrtLPrev(iter);
	result = *(int*)SrtLGetData(iter);
	if(2 != result)
	{
		flag = 0;
	}
	
	SrtLDestroy(srtl);
	
	return flag;	
}

int TestMerge()
{
	int flag = 1;
	int result = 0;
	
	int arr1[] = { 5, 7, 15, 35, 50, 64, 120, 200, 250 };
	int arr2[] = {20, 25, 30, 60, 260, 270};
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 9; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 6; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	
	/* first element should be 5 */
	iter = SrtLBegin(dest);
	result = *(int*)SrtLGetData(iter);
	if(5 != result)
	{
		flag = 0;
	}
	
	iter = SrtLNext(iter);
	result = *(int*)SrtLGetData(iter);
	if(7 != result)
	{
		flag = 0;
	}
	
	SrtLDestroy(dest);
	SrtLDestroy(src);
	
	return flag;
}

int TestMerge2()
{
	int flag = 1;

	int arr1[] = { 4, 6, 10, 15 };
	int arr2[] = { 2, 12, 20, 25 };
	int arr3[] = { 2, 4, 6, 10, 12, 15, 20, 25 };
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 4; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	iter = SrtLBegin(dest);
	
	
	/* compare dest and arr3 */
	for(i = 0; i < 8; ++i)
	{
		if(*(int*)SrtLGetData(iter) != arr3[i])
		{
			flag = 0;
		}
		iter = SrtLNext(iter);
	}
	
	return flag;
}

int TestMerge3()
{
	int flag = 1;
	
	int arr1[] = { 4, 6, 10, 15, 30 };
	int arr2[] = { 2, 12, 20, 25 };
	int arr3[] = { 2, 4, 6, 10, 12, 15, 20, 25, 30 };
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 5; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	iter = SrtLBegin(dest);
	
	/* compare dest and arr3 */
	for(i = 0; i < 9; ++i)
	{
		if(*(int*)SrtLGetData(iter) != arr3[i])
		{
			flag = 0;
		}
		iter = SrtLNext(iter);
	}
	
	return flag;
}

int TestMerge4()
{
	int flag = 1;
	
	int arr1[] = { 2, 6, 10, 15 };
	int arr2[] = { 4, 12, 20, 25 };
	int arr3[] = { 2, 4, 6, 10, 12, 15, 20, 25 };
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 4; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	
	iter = SrtLBegin(dest);
	/* compare dest and arr3 */
	for(i = 0; i < 8; ++i)
	{
	
		if(*(int*)SrtLGetData(iter) != arr3[i])
		{
			flag = 0;
		}
		iter = SrtLNext(iter);
	}
	
	return flag;
}

int TestMerge5()
{
	int flag = 1;
	
	int arr1[] = { 10 };
	int arr2[] = { 4, 12, 20, 25 };
	int arr3[] = { 4, 10, 12, 20, 25 };
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 1; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 4; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	
	iter = SrtLBegin(dest);
	/* compare dest and arr3 */
	for(i = 0; i < 5; ++i)
	{
	
		if(*(int*)SrtLGetData(iter) != arr3[i])
		{
			flag = 0;
		}
		iter = SrtLNext(iter);
	}
	
	return flag;
}


int TestMerge6()
{
	int flag = 1;
	
	int arr1[] = { 2, 6, 10, 15 };
	int arr2[] = { 4 };
	int arr3[] = { 2, 4, 6, 10, 15 };
	
	sit_t iter = NULL;
	int i = 0;
	
	srtl_t *src = SrtLCreate(Sort, (void*)1); 
	srtl_t *dest = SrtLCreate(Sort, (void*)1);
	
	for(i = 0; i < 4; ++i) /* Insert the whole array */
	{
		iter = SrtLInsert(src, &arr1[i]);
	}
	
	for(i = 0; i < 1; ++i)
	{
		iter = SrtLInsert(dest, &arr2[i]);
	}
	
	SrtLMerge(dest, src);
	
	iter = SrtLBegin(dest);
	/* compare dest and arr3 */
	for(i = 0; i < 5; ++i)
	{
	
		if(*(int*)SrtLGetData(iter) != arr3[i])
		{
			flag = 0;
		}
		iter = SrtLNext(iter);
	}
	
	return flag;
}

int BenTest()
{
size_t i = 0;
	
	srtl_t *list1 = NULL;
	srtl_t *list2 = NULL;
	srtl_t *list3 = NULL;
	size_t data_arr1[] = {1,4,6,7};
	size_t data_arr2[] = {1,2,3};
	size_t data_arr3[] = {1,1,2,3,4,6,7};
	
	/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MERGE OF SMALL LIST INTO BIG LIST*/
	list1 = SrtLCreate(Sort, (void*)1);
	list2 = SrtLCreate(Sort, (void*)1);
	list3 = SrtLCreate(Sort, (void*)1);
	
	for (i = 0; sizeof(data_arr1)/sizeof(data_arr1[0])> i; ++i )
	{
		SrtLInsert(list1, &data_arr1[i]);
	}
	
	for (i = 0; sizeof(data_arr2)/sizeof(data_arr2[0])> i; ++i )
	{
		SrtLInsert(list2, &data_arr2[i]);
	}

	for (i = 0; sizeof(data_arr3)/sizeof(data_arr3[0])> i; ++i )
	{
		SrtLInsert(list3, &data_arr3[i]);
	}
	
	
	printf("Before Merge: \n\n");
	PrintList(list1);
	
	SrtLMerge(list1, list2);

	printf("After Merge : \n\n");
	PrintList(list1);
	return 1;

}

int main()
{
	RUN_TEST(TestFlow);
	RUN_TEST(TestFind);
	RUN_TEST(TestForEach);
	RUN_TEST(TestMerge);
	RUN_TEST(TestMerge2);
	RUN_TEST(TestMerge3);
	RUN_TEST(TestMerge4);
	RUN_TEST(TestMerge5);
	RUN_TEST(TestMerge6);
	RUN_TEST(BenTest);
	
	return 1;
}
