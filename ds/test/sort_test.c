#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/sort.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 


int Compare(const void *a, const void *b)
{
	int *first = (int*)a;
	int *second = (int*)b;
	
	return *first - *second;
}

size_t KeyToNum(const void *data, const void *args)
{
	(void) args;
	
	return (size_t)*(int*)data; 
}

void Print(int *arr, size_t len)
{
	size_t i = 0;
	for(i = 0; i < len; ++i)
	{
		printf("%d \n", arr[i]);
	}
}

int BubbleTest()
{
	clock_t duration; 
	
	int arr[5000];
	int i = 0;
	int flag = 1;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand();
	}
	
	duration = clock();
	BubbleSort(arr, 5000, 4, Compare);
	printf("Duration of Bubble: %f\n", (double)(clock() - duration) / CLOCKS_PER_SEC);
	
	for(i = 1; i < 5000; ++i)
	{
		if(arr[i] < arr[i - 1])
		{
			flag = 0;
		}
	}
	
	return flag;
	
	return 1;
}

int SelectionTest()
{
	
	clock_t duration;
	int arr[5000];
	int i = 0;
	int flag = 1;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand();
	}
	
	duration = clock();	
	SelectionSort(arr, 5000, 4, Compare);
	printf("Duration of Selection: %f\n", (double)(clock() - duration) / CLOCKS_PER_SEC);
	
	for(i = 1; i < 5000; ++i)
	{
		if(arr[i] < arr[i - 1])
		{
			flag = 0;
		}
	}
	
	return flag;
	
}

int InsertionTest()
{
	clock_t duration;
	int arr[5000];
	int i = 0;
	int flag = 1;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand();
	}
	
	duration = clock();
	InsertionSort(arr, 5000, 4, Compare);
	printf("Duration of Insertion: %f\n", (double)(clock() - duration) / CLOCKS_PER_SEC);
	
	for(i = 1; i < 5000; ++i)
	{
		if(arr[i] < arr[i -1])
		{
			flag = 0;
		}
	}
	
	return flag;
}

int CountingTest()
{
	int arr[5000];
	int i = 0;
	int flag = 1;
	clock_t duration;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand() % 2000;
	}
	
	duration = clock();
	CountingSort(arr, 5000, 4, KeyToNum, (void*)1, 2000);
	printf("Duration of Counting: %f\n", (double)(clock() - duration) / CLOCKS_PER_SEC);
	
	for(i = 1; i < 5000; ++i)
	{
		if(arr[i] < arr[i -1])
		{
			flag = 0;
		}
	}
	
	return flag;
}
/*
int RadixTest()
{
	int arr[5000];
	int i = 0;
	int flag = 1;
	clock_t duration;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand() % 5000;
	}
	
	duration = clock();
	RadixSort(arr, 5000, 4, KeyToNum, (void*)1, 2);
	printf("Duration of Radix: %f\n", (double)(clock() - duration) / CLOCKS_PER_SEC);
	
	for(i = 1; i < 5000; ++i)
	{
		if(arr[i] < arr[i -1])
		{
			flag = 0;
		}
	}
	
	return flag;
	

}
*/

int QuickSortTest()
{
	int arr[100];
	int i = 0;
	int flag = 1;
	
	for(i = 0; i < 100; ++i)
	{
		arr[i] = rand() % 1000;
	}
	
	QuickSort(arr, 100, ASC);
	
	for(i = 0; i < 99; ++i)
	{
		if(arr[i] > arr[i + 1])
		{
			flag = 0;
		}	
	}
	
	return flag;
}

int compare(const void *data1, const void *data2)
{
	return *(int*)data1 - *(int*)data2;
}

int QuickSortBigTest()
{
	int arr[100000];
	int arr2[100000];
	
	int i = 0;
	int flag = 1;
	
	clock_t before;
	clock_t after;
	
	(void)arr2;
	
	for(i = 0; i < 100000; ++i)
	{
		arr[i] = rand() % 100000;
	}
	
	for(i = 0; i < 100000; ++i)
	{
		arr2[i] = arr[i];
	}
	
	before = clock();
	QuickSort(arr, 100000, ASC);
	after = clock();
	
	printf("my sort: %f\n", (float)(after - before) / CLOCKS_PER_SEC); 
	
	before = clock();
	qsort(arr, 100000, sizeof(int), compare);
	after = clock();
	
	printf("their sort: %f\n", (float)(after - before) / CLOCKS_PER_SEC); 
	
	for(i = 0; i < 99999; ++i)
	{
		if(arr[i] > arr[i + 1])
		{
			flag = 0;
		}
	}
	
	return flag;		

}


int BinarySearchTest()
{
	int arr[100];
	int i = 0;
	int res = 0;
	size_t index;
	int flag = 1;
	
	for(i = 0; i < 50; ++i)
	{
		arr[i] = rand() % 100;
	}
	arr[50] = 103;
	
	for(i = 51; i < 100; ++i)
	{
		arr[i] = 104 + rand() % 100;
	}
	
	QuickSort(arr, 100, ASC);
	
	res = BinarySearch(arr, 103, 100, &index);
	
	if(res != 1 || index != 50)
	{
		flag = 0;
	}
	
	arr[0] = 3;
	
	
	for(i = 1; i < 100; ++i)
	{
		arr[i] = 4 + rand() % 100;
	}
	
	QuickSort(arr, 100, ASC);
	
	res = BinarySearch(arr, 3, 100, &index);
	
	if(res != 1 || index != 0)
	{
		flag = 0;
	}
	
	return flag;
}

int JumpSearchTest()
{
	int arr[100];
	int i = 0;
	int res = 0;
	size_t index;
	int flag = 1;
	
	for(i = 0; i < 50; ++i)
	{
		arr[i] = rand() % 100;
	}
	arr[50] = 103;
	
	for(i = 51; i < 100; ++i)
	{
		arr[i] = 104 + rand() % 100;
	}
	
	QuickSort(arr, 100, ASC);
	
	res = JumpSearch(arr, 103, 100, &index);
	
	if(res != 1 || index != 50)
	{
		flag = 0;
	}
	
	arr[0] = 3;
	
	
	for(i = 1; i < 100; ++i)
	{
		arr[i] = 4 + rand() % 100;
	}
	
	QuickSort(arr, 100, ASC);
	
	res = JumpSearch(arr, 3, 100, &index);
	
	if(res != 1 || index != 0)
	{
		flag = 0;
	}
	
	return flag;
}


int main()
{
/*	RUN_TEST(BubbleTest);*/
/*	RUN_TEST(SelectionTest);*/
/*	RUN_TEST(InsertionTest);*/
/*	RUN_TEST(CountingTest);*/
	/*RUN_TEST(RadixTest);*/
	RUN_TEST(QuickSortTest)
	RUN_TEST(BinarySearchTest)
	RUN_TEST(JumpSearchTest)
	RUN_TEST(QuickSortBigTest)
	
	return 1;
}
