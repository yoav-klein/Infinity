
/*********************************************
*		Variable size memory allocation
*		Author: Yoav Klein
*		Reviewer: Daniel Maizel
*		Date: 23.6.19
***********************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* stdlib */

#define HDR sizeof(blk_header_t)
#define WRD 8

typedef struct blk_header
{
	long block_size;
	#ifndef NDEBUG
	long magic_num;
	#endif
}	blk_header_t;

typedef struct vsma
 {
	char c;
} vsma_t;

vsma_t *VSMAInit(void *pool, size_t size)
{
	blk_header_t *block = NULL;

	assert(NULL != pool);
	
	block = (blk_header_t*)pool;
		
	block->block_size = size - size % WRD - 2 * HDR;	
	#ifndef NDEBUG
	block->magic_num = 0xdeadbeef;
	#endif
	
	((blk_header_t*)((char*)pool + size - size % WRD - HDR))->block_size = 0;
	
	return pool;
}

static void VSMADefrag(vsma_t *vsma)
{
	char *runner = (char*)vsma;
	long *value = (long*)runner;
	
	while(0 != *value)
	{
		if(*value > 0)
		{	/* if next block is free, connect them */
			if(*(long*)(runner + HDR + *value) > 0)
			{
				*value += *(long*)(runner + HDR + *value) + HDR;
			}
			else /* else move on */
			{
				runner += HDR + *value;
				value = (long*)runner;
			}
		}
		else
		{
			runner += HDR + *(long*)runner * (-1);
			value = (long*)runner;
		}
	}
}

void *VSMAAlloc(vsma_t *vsma, size_t block_size)
{
	char *runner = NULL;
	long *value = NULL;
	block_size = block_size + (WRD - (block_size % WRD)) % WRD;

	assert(NULL != vsma);

	runner = (char*)vsma;
	value = (long*)runner;
	
	while(0 != *value)
	{
		if(0 > *value)
		{
			runner = runner + HDR + *(long*)runner * (-1);
			value = (long*)runner;
		} 
		/* if there's enough space for this requested size
		and another at lease one aligned block after it, that is -
		size of header and word size space */
		else if(block_size + HDR + WRD <= (size_t)*value)
		{
			((blk_header_t*)(runner + HDR + block_size))->block_size =
			 *value - block_size - HDR;
			((blk_header_t*)runner)->block_size = block_size * (-1);
			
			#ifndef NDEBUG
			((blk_header_t*)(runner + HDR + block_size))->magic_num = 0xdeadbeef;
			((blk_header_t*)runner)->magic_num = 0xdeadbeef;
			#endif
			
			return (runner + HDR);	
		}
		/* if there is only enough space for just this requested size,
		don't put another header after it, and return this address.*/
		else if(block_size <= (size_t)*value)
		{
			*value *= -1;
			
			return (runner + HDR);
		}
		/* if there isn't enough even for this requested size, check
		if the next block is free. if is, connect them and run again. */
		else if(*(long*)(runner + HDR + *value) > 0)
		{
			*value += *(long*)(runner + HDR + *value) + WRD;
		}
		/* if the next isn't free, move on to the next block and keep searching. */
		else
		{
			runner += HDR + *value;
			value = (long*)runner;
		}
	} 
	
	/* return null if there wasn't a place found. */
	return NULL;
}

void VSMAFree(void *block)
{
	assert(((blk_header_t*)((char*)block - HDR))->magic_num == 0xdeadbeef);
	
	#ifndef NDEBUG
	((blk_header_t*)((char*)block - HDR))->magic_num = 0;
	#endif
	
	((blk_header_t*)((char*)block - HDR))->block_size *= -1;
}

size_t VSMACount(vsma_t *vsma)
{
	char *runner = (char*)vsma;
	long *value = (long*)runner;
	size_t count = 0;
	
	assert(NULL != vsma);
	
	runner = (char*)vsma;
	
	VSMADefrag(vsma);
	
	while(0 != *value) 
	{
		if(0 < *value)
		{
			count += *value;
			runner += HDR + *value;
			value = (long*)runner;
		}
		else
		{
			runner += HDR + *value * (-1);
			value = (long*)runner;
		}
	}
	
	return count;
}

size_t VSMAFindLargestFree(vsma_t *vsma)
{
	char *runner = NULL;
	long *value = (long*)runner;
	long largest = 0;
	
	assert(NULL != vsma);
	
	runner = (char*)vsma;
	
	VSMADefrag(vsma);	
		
	while(0 != *value)
	{
		if(0 < *value)
		{
			if(*value > largest)
			{
				largest = *value;
			}
			runner += HDR + *value;
			value = (long*)runner;
		}
		else
		{
			runner += HDR + *value * (-1);
			value = (long*)runner;
		}
	}
	
	return largest;
}

