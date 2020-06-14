#include <stdio.h>
#include <stdlib.h>

#include "../include/avl.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

struct person
{
	int key;
	double height;
};

int AddHeight(void *data, const void *params)
{
	(void)params;
	
	((struct person*)data)->height += 1;
	
	return 0;
}

int PrintPeople(void *data, const void *params)
{
	struct person person = *(struct person*)data;
	
	(void)params;
	printf("key: %d   height: %f\n", person.key, person.height);
	
	return 0;
}

int NumberNotFound(int *arr, int num, int len)
{
	int i = 0;
	
	for(i = 0; i < len; ++i)
	{
		if(arr[i] == num)
		{
			return 0;
		}
	}
	
	return 1;
}

int ComparePerson(const void *data1, const void *data2, const void *params)
{
	(void)params;
	
	return ((struct person*)data1)->key - ((struct person*)data2)->key;
}

int Compare(const void *data1, const void *data2, const void *params)
{
	(void)params;
	
	return *(int*)data1 - *(int*)data2;
}

int InsertAndRemoveTest()
{
	avl_t *avl = AVLCreate(Compare, NULL);
	void *find_test = NULL;
	int flag = 1;
	size_t result = 0;
	
	int arr[20] = { 4, 54, 192, 55, 665, 543, 6543, 23, 12, 33, 484,
					32, 11, 65, 45, 94, 213, 4234, 43 };
	size_t i = 0;
	
	for(i = 0; i < 20 ; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}
	
	
	find_test = AVLFind(avl, &arr[0]);
	
	if(*(int*)find_test != 4)
	{
		flag = 0;
	}
	
	find_test = AVLFind(avl, &arr[6]);
	if(*(int*)find_test != 6543)
	{
		flag = 0;
	}
	
	result = AVLGetSize(avl);
	if(20 != result)
	{
		flag = 0;
	}
	
	result = AVLGetHeight(avl);
	if(4 != result)
	{
		flag = 0;
	}
	
	AVLDestroy(avl);
	
	return flag;
}

int FindForEachTest()
{
	avl_t *avl = AVLCreate(Compare, NULL);
	int rand_num = 0;
	int arr[30];
	struct person people[30];
	size_t i = 0;
	double height_test = 0;
	struct person find_test;
	int flag = 1;
	
	for(i = 0; i < 30; ++i)
	{
		do
		{
			rand_num = rand() % 100;
		}
		while(0 == NumberNotFound(arr, rand_num, i + 1));
		
		arr[i] = rand_num;
	}
		
	for(i = 0; i < 30; ++i)
	{
		people[i].key = arr[i];
		people[i].height = (160 + (double)(rand() % 30)) / 100 ;
	}
	
	height_test = people[10].height;
	
	for(i = 0; i < 30; ++i)
	{
		AVLInsert(avl, &people[i]);
	}
	
	AVLForEach(avl, AddHeight, NULL);
	
	find_test = *(struct person*)(AVLFind(avl, &arr[10]));
	
	if(find_test.height != height_test + 1)
	{
		flag = 0;
	}
	
	AVLDestroy(avl);
	return flag;
}

int StressTest()
{
	avl_t *avl = AVLCreate(Compare, NULL);
	int arr[50000];
	size_t i = 0;
	int rand_num = 0;
	int flag = 1;
	size_t result = 0;
		
	for(i = 0; i < 50000; ++i)
	{
		do
		{
			rand_num = rand();
		}
		while(0 == NumberNotFound(arr, rand_num, i + 1));
		
		arr[i] = rand_num;
	}
	
	for(i = 0; i < 50000; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}
	
	result = AVLGetSize(avl);
	if(50000 != result)
	{
		flag = 0;
	}
	
	result = AVLGetHeight(avl);
	if(result > 20 || result < 15)
	{
		flag = 0;
	}
	
	AVLDestroy(avl);
	
	return flag;
}

int main()
{	
	RUN_TEST(InsertAndRemoveTest);
	RUN_TEST(FindForEachTest);
	RUN_TEST(StressTest);
	
	
	return 0;
}
