#include <stdio.h>
#include <stdlib.h>

#include "../include/bst.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int Add(void *data, const void *args)
{
	*(int*)data += *(int*)args;
	
	return 0;
}

int Compare(const void *data, const void *newdata, const void *params)
{
	(void)params;
	
	return *(int*)data - *(int*)newdata;
}

int FlowTest()
{
	bst_t *bst = BSTCreate(Compare, NULL);
	bst_it_t current = NULL;
	bst_it_t remove = NULL;
	bst_it_t iter = NULL;
	
	size_t count = 0;
	int result = 0;
	int flag = 1;
	
	size_t i = 0;
	int arr[20];	
	
	arr[0] = 200;
	arr[1] = 100;
	arr[2] = 300;
	arr[3] = 150;
	arr[4] = 160;
	arr[5] = 30;
	arr[6] = 2;
	arr[7] = 45;
	arr[8] = 188;
	arr[9] = 750;
	arr[10] = 433;
	arr[11] = 322;
	arr[12] = 122;
	arr[13] = 231;
	arr[14] = 198;
	arr[15] = 11;
	arr[16] = 150;
	arr[17] = 430;
	arr[18] = 55;
	arr[19] = 20;	
	
	/* check count and is empty on empty bst */
	result = BSTCount(bst);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = BSTIsEmpty(bst);
	if(1 != result)
	{
		flag = 0;
	}
	
	/* insert elements to bst, check count and is empty. */
	for(i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	
	result = BSTCount(bst);
	if(19 != result)
	{
		flag = 0;
	}
	
	result = BSTIsEmpty(bst);
	if(0 != result)
	{
		flag = 0;
	}
	
	/* test Begin, End, Next, Prev and GetData */
	current = BSTBegin(bst);
	
	while(BSTEnd(bst) != BSTNext(current))
	{
		if(*(int*)BSTGetData(current) >= *(int*)BSTGetData(BSTNext(current)))
		{
			flag = 0;
		}
		current = BSTNext(current);
		++count;
	}
	
	if(18 != count)
	{
		flag = 0;
	}
	
	/* Same thing, from end to start*/
	
	current = BSTFind(bst, &arr[9]); /* the biggest element */
	count = 0;
	while(BSTBegin(bst) != current)
	{
		if(*(int*)BSTGetData(current) <= *(int*)BSTGetData(BSTPrev(current)))
		{
			flag = 0;
		}
		current = BSTPrev(current);
		++count;
	}
	
	if(18 != count)
	{
		flag = 0;
	}
	
	/* Test Find */
	current = BSTFind(bst, &arr[10]);
	result = *(int*)BSTGetData(current);
	if(433 != result)
	{
		flag = 0;
	}
	
	current = BSTFind(bst, &arr[0]);
	result = *(int*)BSTGetData(current);
	if(200 != result)
	{
		flag = 0;
	}
	
	/* Remove a leaf, check count, run on the whole tree */
	remove = BSTFind(bst, &arr[18]);
	BSTRemove(remove);
	
	result = BSTCount(bst);
	if(18 != result)
	{
		flag = 0;
	}
	
	current = BSTBegin(bst);
	count = 0;
	while(BSTEnd(bst) != BSTNext(current))
	{
		if(*(int*)BSTGetData(current) >= *(int*)BSTGetData(BSTNext(current)))
		{
			flag = 0;
		}
		current = BSTNext(current);
		++count;
	}
	
	if(17 != count)
	{
		flag = 0;
	}
	
	
	/* Remove a node with 1 child, again.. */
	remove = BSTFind(bst, &arr[15]);
	BSTRemove(remove);
	
	result = BSTCount(bst);
	if(17 != result)
	{
		flag = 0;
	}
	
	current = BSTBegin(bst);
	count = 0;
	while(BSTEnd(bst) != BSTNext(current))
	{
		if(*(int*)BSTGetData(current) >= *(int*)BSTGetData(BSTNext(current)))
		{
			flag = 0;
		}
		current = BSTNext(current);
		++count;
	}
	
	if(16 != count)
	{
		flag = 0;
	}
	
	/* Remove a node with 2 children, again.. */
	remove = BSTFind(bst, &arr[5]);
	BSTRemove(remove);
	
	result = BSTCount(bst);
	if(16 != result)
	{
		flag = 0;
	}
	
	current = BSTBegin(bst);
	count = 0;
	while(BSTEnd(bst) != BSTNext(current))
	{
		if(*(int*)BSTGetData(current) >= *(int*)BSTGetData(BSTNext(current)))
		{
			flag = 0;
		}
		current = BSTNext(current);
		++count;
	}
	
	if(15 != count)
	{
		flag = 0;
	}
	
	count = BSTCount(bst);
	
	/* Remove one by one from the beginning */
	
	current = BSTBegin(bst);
	while(BSTEnd(bst) != BSTNext(current))
	{
		remove = current;
		current = BSTNext(current);
		
		BSTRemove(remove);
		
		if(--count != BSTCount(bst))
		{
			flag = 0;
		}
		/*
		 each removal, run on the tree and check it's ordered. */
		iter = BSTBegin(bst);
		while(BSTNext(iter) != BSTEnd(bst))
		{
			if(*(int*)BSTGetData(iter) >= *(int*)BSTGetData(BSTNext(iter)))
			{
				flag = 0;
			}
			iter = BSTNext(iter);
		}
	}
	
	BSTDestroy(bst);
	return flag;
}

int FiveKTest()
{
	bst_t *bst = BSTCreate(Compare, NULL);
	bst_it_t current = NULL;
	bst_it_t next = NULL;
	
	int flag = 1;
	size_t count = 0;
	
	size_t i = 0;
	int arr[5000];	
	(void)flag;
	
	for(i = 0; i < 5000; ++i)
	{
		arr[i] = rand() % 10000000;
		BSTInsert(bst, &arr[i]);
	}
	
	count = BSTCount(bst);
	printf("Count: %lu\n", count);
	
	current = BSTBegin(bst);
	for(i = 0; i < count - 1; ++i)
	{
		next = BSTNext(current);
		if(*(int*)BSTGetData(next) <= *(int*)BSTGetData(current))
		{	
			flag = 0;
		}
		
		current = next;
	}
		
	BSTDestroy(bst);
	
	return 1;
}

int PrevOnTheSmallest()
{
	bst_t *bst = BSTCreate(Compare, NULL);
	bst_it_t current = NULL;
	
	int flag = 1;
	
	size_t i = 0;
	int arr[6];	
	
	arr[0] = 200;
	arr[1] = 100;
	arr[2] = 300;
	arr[3] = 150;
	arr[4] = 160;
	arr[5] = 30;
	arr[6] = 2;
	
	for(i = 0; i < 7; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	
	current = BSTFind(bst, &arr[6]);
	current = BSTPrev(current);
	
	BSTDestroy(bst);
	
	return flag;
}

int main()
{
	RUN_TEST(FlowTest);
	RUN_TEST(FiveKTest);
	RUN_TEST(PrevOnTheSmallest);

	return 0;
}
