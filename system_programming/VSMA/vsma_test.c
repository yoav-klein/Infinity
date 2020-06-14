
/*********************************************
*		Variable size memory allocation
*		Author: Yoav Klein
*		Reviewer: Daniel Maizel
*		Date: 23.6.19
***********************************************/

#include <stdio.h> /* pr */
#include <stdlib.h>

#include "vsma.h"

int FlowTest()
{
	int flag = 1;
	int *space1 = NULL;
	int *space2 = NULL;
	int *space3 = NULL;
	int *space4 = NULL;
	size_t result = 0;
	
	void *pool = malloc(150);
	
	int i = 0;
	
	vsma_t* vsma = VSMAInit(pool, 150);
	
	/* count free and check largest */	
	result = VSMAFindLargestFree(vsma);
	if(128 != result)
	{
		flag = 0;
	}
	
	result = VSMACount(vsma);
	if(128 != result)
	{
		flag = 0;
	}
	
	/* Alloc 50 bytes. put values and check count and largest */
	space1 = (int*)VSMAAlloc(vsma, 50);
	
	for(i = 0; i < 12; ++i)
	{
		space1[i] = i + 1;	
	}
	
	/* check everything is fine */
	for(i = 0; i < 12; ++i)
	{
		if(space1[i] != i + 1)
		{
			flag = 0;
		}
	}
	
	result = VSMACount(vsma);
	if(64 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(64 != result)
	{
		flag = 0;
	}
	
	/* Alloc 24. check count and largest */
	space2 = (int*)VSMAAlloc(vsma, 24);
	
	for(i = 0; i < 6; ++i)
	{
		space2[i] = i + 1;
	}
	
	for(i = 0; i < 6; ++i)
	{
		if(space2[i] != i + 1)
		{
			flag = 0;
		}
	}
	
	result = VSMACount(vsma);
	if(32 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(32 != result)
	{
		flag = 0;
	}
	
	/* alloc 4, check again */
	space3 = (int*)VSMAAlloc(vsma, 4);
		
	*space3 = 10;
	if(10 != *space3)
	{
		flag = 0;
	}	
	
	result = VSMACount(vsma);
	if(16 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(16 != result)
	{
		flag = 0;
	}
	
	/* Alloc 16, same thing */
	
	space4 = (int*)VSMAAlloc(vsma, 16);
	for(i = 0; i < 4; ++i)
	{
		space4[i] = i + 1;
	}
	
	for(i = 0; i < 4; ++i)
	{
		if(space4[i] != i + 1)
		{
			flag = 0;
		}
	}
	
	/* free space 4 and then space1. */
	VSMAFree(space4);
	result = VSMAFindLargestFree(vsma);
	if(16 != result)
	{
		flag = 0;
	}
	
	result = VSMACount(vsma);
	if(16 != result)
	{
		flag = 0;
	}
	
	VSMAFree(space1);
	
	result = VSMACount(vsma);
	if(72 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(56 != result)
	{
		flag = 0;
	}
	
	/* free space2 */
	
	VSMAFree(space2);
	result = VSMACount(vsma);
	if(104 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(88 != result)
	{
		flag = 0;
	}
	
	/* free space3 */
	VSMAFree(space3);
	
	result = VSMACount(vsma);
	if(128 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(128 != result)
	{
		flag = 0;
	}
	
	/* alloc more space the available */
	space1 = VSMAAlloc(vsma, 137);
	if(NULL != space1)
	{
		flag = 0;
	}
	
	return flag;
	
}

int DebugTest()
{
	int flag = 1;
	int *space1 = NULL;
	int *space2 = NULL;
	int *space3 = NULL;
	
	size_t result = 0;
	
	void *pool = malloc(150);
	
	int i = 0;
	
	vsma_t* vsma = VSMAInit(pool, 150);
	
	/* check count and largest */
	result = VSMACount(vsma);
	if(112 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(112 != result)
	{
		flag = 0;
	}
	
	/* Alloc 56, find largest and count */
	space1 = (int*)VSMAAlloc(vsma, 56);
	
	result = VSMAFindLargestFree(vsma);
	if(40 != result)
	{
		flag = 0;
	}
	
	result = VSMACount(vsma);
	if(40 != result)
	{
		flag = 0;
	}
	
	/* put values, check everything is ok */
	for(i = 0; i < 14; ++i)
	{
		space1[i] = i + 1;
	}
	
	for(i = 0; i < 14; ++i)
	{
		if(i + 1 != space1[i])
		{
			flag = 0;
		} 
	}
	
	/* Alloc 16 */
	space2 = (int*)VSMAAlloc(vsma, 16);
	
	result = VSMACount(vsma);
	if(8 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(8 != result)
	{
		flag = 0;
	}
	
	for(i = 0; i < 4; ++i)
	{
		space2[i] = i + 1;
	}
	
	for(i = 0; i < 4; ++i)
	{
		if(i + 1 != space2[i])
		{
			flag = 0;
		}
	}
	
	/* Alloc 8, since there's not enough space for another header,
	space should be over */
	space3 = (int*)VSMAAlloc(vsma, 8);
	
	result = VSMACount(vsma);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(0 != result)
	{
		flag = 0;
	}
	
	/* Free space3 */
	VSMAFree(space3);
	
	result = VSMACount(vsma);
	if(8 != result)
	{
		flag = 0;
	}
	
	result = VSMAFindLargestFree(vsma);
	if(8 != result)
	{
		flag = 0;
	}
	
	/* Free space1 */
	VSMAFree(space1);
	result = VSMAFindLargestFree(vsma);
	if(56 != result)
	{
		flag = 0;
	}
	
	result = VSMACount(vsma);
	if(64 != result)
	{
		flag = 0;
	}
	
	/* Free space2 */
	VSMAFree(space2);
	
	result = VSMAFindLargestFree(vsma);
	if(112 != result)
	{
		flag = 0;
	}
	
	result = VSMACount(vsma);
	if(112 != result)
	{
		flag = 0;
	}
	
	VSMAFree(space3);
	
	free(pool);
	
	return flag;
}

int Test()
{
	
	int flag = 1;
	int *space1 = NULL;
	int *space2 = NULL;
	int *space3 = NULL;
	int *space4 = NULL;
	int *space5 = NULL;
	
	size_t result = 0;
	
	void *pool = malloc(150);
	
	int i = 0;
	
	vsma_t* vsma = VSMAInit(pool, 150);
	
	result = VSMACount(vsma);
	
	space1 = (int*)VSMAAlloc(vsma, 16);
	space2 = (int*)VSMAAlloc(vsma, 8);
	space3 = (int*)VSMAAlloc(vsma, 8);
	space4 = (int*)VSMAAlloc(vsma, 8);
	space5 = (int*)VSMAAlloc(vsma, 8);

	VSMAFree(space1);
	VSMAFree(space2);
	VSMAFree(space3);
	VSMAFree(space4);
	VSMAFree(space5);
	
	return result == VSMACount(vsma);


}

int main()
{
	printf("%d\n", Test());	
	return 1;
}


