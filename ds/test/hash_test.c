/************************************************
*		Hash Table
*		Author: Yoav Klein
*		Reviewer: Eldad Carin
*		Date: 16.7.19	
***************************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h> /*  */
#include <string.h>
#include "hash.h"

#define RANGE 50

#define STAT_RANGE 10

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int Compare(const void *data, const void *key)
{
	return *(int*)data == *(int*)key;
}

size_t DictionaryHash(void *key)
{
	size_t res = 1;
    char *runner = (char*)key;
  
    while (*runner != '\0')
    {
    	res = res * (size_t)*runner;
    	++runner;
    }
    
   
    return res % 10000;
}

int DictionaryCompare(const void *data, const void *key)
{
	char *str1 = (char*)data;
	char *str2 = (char*)key;
	if(!strcmp(str1, str2))
	{
		return 1;
	}
	
	return 0;
}

size_t HashFunction(void *key)
{
	int num = *(int*)key;
	
	return (num % RANGE);
}

size_t StatHashFunction(void *key)
{
	int num = *(int*)key;
	
	return (num % STAT_RANGE);
}


int PrintHashTable(void *data, const void *params)
{
	(void)params;
	
	printf("%d -> ", *(int*)data);
	
	return 0;
}

int SumArray(void *data, const void *params)
{
	*(int*)params += *(int*)data;

	return 0;
}

int CreateDestroyTest()
{
	hash_t *table = HashCreate(Compare, HashFunction, RANGE);
	
	HashDestroy(table);
	
	return 1;
}

int FlowTest()
{
	hash_t *table = HashCreate(Compare, HashFunction, RANGE);
	int i = 0;
	int arr[20];
	int *find_test = NULL;
	int flag = 1;
	size_t res = 0;
	
	for(i = 0; i < 20; ++i)
	{
		arr[i] = i * 5;
	}
	
	/* Test size and IsEmpty */
	res = HashSize(table);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = HashIsEmpty(table);
	if(1 != res)
	{
		flag = 0;
	}
	
	for(i = 0; i < 20; ++i)
	{
		HashInsert(table, &arr[i]);
	}
	
	/*  Test size and IsEmpty */
	res = HashSize(table);
	if(20 != res)
	{
		flag = 0;
	}
	
	res = HashIsEmpty(table);
	if(0 != res)
	{
		flag = 0;
	}
	
	/* Test Find */
	find_test = (int*)HashFind(table, &arr[10]);
	if(arr[10] != *find_test)
	{
		flag = 0;
	}
	
	/* remove, test find and size, shouldn't be found */ 
	HashRemove(table, &arr[10]);
	find_test = (int*)HashFind(table, &arr[10]);
	
	if(NULL != find_test)
	{
		flag = 0;
	}
	
	res = HashSize(table);
	if(19 != res)
	{
		flag = 0;
	}
	
	HashDestroy(table);
	
	return flag;
}

int ForEachTest()
{
	hash_t *table = HashCreate(Compare, HashFunction, RANGE);
	int i = 0;
	int arr[20];
	int flag = 1;
	size_t sum = 0;
	size_t fe_sum = 0;
	
	for(i = 0; i < 20; ++i)
	{
		arr[i] = i * 5;
		sum += arr[i];
	}
	
	for(i = 0; i < 20; ++i)
	{
		HashInsert(table, &arr[i]);
	}
	
	HashForEach(table, SumArray, &fe_sum);
	
	if(fe_sum != sum)
	{
		flag = 0;
	}
	
	HashDestroy(table);
	
	return flag;
}

int StatisticsTest()
{
	hash_t *table = HashCreate(Compare, StatHashFunction, STAT_RANGE);
	int i = 0;
	int arr[20];
	int flag = 1;
	double result;
	size_t size = 0;
	
	for(i = 0; i < 20; ++i)
	{
		arr[i] = i;
	}
	
	for(i = 0; i < 20; ++i)
	{
		HashInsert(table, &arr[i]);
	}
	
	result = HashSD(table);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = HashLoad(table);
	if(2 != result)
	{
		flag = 0;
	}
	
	for(i = 0; i < 20; ++i)
	{
		HashRemove(table, &arr[i]);
	}
	
	size = HashSize(table);
	if(0 != size)
	{
		flag = 0;
	}
	
	arr[0] = 21;
	arr[1] = 54;
	arr[2] = 23;
	arr[3] = 11;
	arr[4] = 53;
	arr[5] = 67;
	arr[6] = 10;
	arr[7] = 90;
	arr[8] = 50;
	arr[9] = 3;
	arr[10] = 25;
	arr[11] = 30;
	arr[12] = 44;
	
	for(i = 0; i < 13; ++i)
	{
		HashInsert(table, &arr[i]);
	}
	
	result = HashSD(table);
	if(1.345362 > result || 1.345363 < result)
	{
		flag = 0;	
	}
	
	result = HashLoad(table);
	if(1.3 != result)
	{
		flag = 0;
	}
	
	HashDestroy(table);
	
	return flag;
}

int DictionaryTest()
{
	hash_t *table = HashCreate(DictionaryCompare, DictionaryHash, 10000);
	FILE *file;
	char *dictionary = (char*)malloc(971579);
	char *runner = dictionary;
	size_t count = 0;
	char c = 0;
	char *test = "Aaron";
	char *test2 = "malls";
	char *res = NULL;
	int flag = 1;
	
	file = fopen("/usr/share/dict/words", "r");
	if(NULL == file)
	{
		return 0;
	}
	
	while(EOF != c)
	{
		c = getc(file);
		if('\n' == c)
		{
			*runner = '\0';
			HashInsert(table, runner - count);
			count = 0;
		}
		else
		{
			*runner = c;
			++count;
		}
		++runner;
	}
	
	fclose(file);
	
	res = HashFind(table, test);
	
	if(strcmp("Aaron", res) != 0)
	{
		flag = 0;
	}
	
	res = HashFind(table, test2);
	if(strcmp("malls", res) != 0)
	{
		flag = 0;
	}
	
  	free(dictionary);
  	
  	HashDestroy(table);
	return flag;
}

int main()
{
	RUN_TEST(CreateDestroyTest)
	RUN_TEST(FlowTest)
	RUN_TEST(ForEachTest)
	RUN_TEST(StatisticsTest)
	RUN_TEST(DictionaryTest)
	
	return 0;
}
