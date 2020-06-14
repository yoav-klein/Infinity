/**********************************************
*	Fixed Sized Allocation Management
*	Author: Yoav Klein
*	Reviewer: Eran Segal
*	Date: 20.6.19
*************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "fsma.h" /* FSMAInit */
#define WORD_SIZE 8

struct fsma
{
	size_t next_free;
	size_t block_size;
	size_t n_blocks;
};

size_t FSMASuggestSize(size_t n_blocks, size_t block_size)
{
	size_t real_block_size = block_size + 
	(WORD_SIZE - (block_size % WORD_SIZE)) % WORD_SIZE + WORD_SIZE; 
	
	return (sizeof(fsma_t) + real_block_size * n_blocks);
}

fsma_t *FSMAInit(void *pool, size_t pool_size, size_t block_size)
{
	
	size_t *iterator = NULL;
	size_t i = 0;
	size_t real_block_size = block_size + 
	(WORD_SIZE - (block_size % WORD_SIZE)) % WORD_SIZE + WORD_SIZE;
	 
	fsma_t *fsma = (fsma_t*)pool;
	
	assert(NULL != pool);
	
	fsma->block_size = real_block_size;
	fsma->n_blocks = (pool_size - sizeof(fsma_t)) / real_block_size;
	fsma->next_free = sizeof(fsma_t);
	
	iterator = (size_t*)pool + sizeof(fsma_t) / WORD_SIZE;
	
	for(i = 0; i < fsma->n_blocks - 1; ++i)
	{
		*iterator = sizeof(fsma_t) + real_block_size * (i + 1);
		iterator = iterator + real_block_size / WORD_SIZE;
	}
	
	*iterator = 0;
	
	return (fsma_t*)pool;
}

void *FSMAAlloc(fsma_t *fsma)
{
	size_t temp = 0;
	size_t *block = 0;
	
	assert(NULL != fsma);	
	
	block = (size_t*)fsma + fsma->next_free / WORD_SIZE;
		
	temp = fsma->next_free;
	fsma->next_free = *block;
	*block = temp;
	
	return (block + 1);
}

void FSMAFree(void *block)
{
	size_t temp = 0;
	size_t offset = 0; 
	size_t *next_free = 0;
	
	assert(NULL != block);
	
	offset = *((size_t*)block - 1);
	next_free = (size_t*)block - 1 - offset / WORD_SIZE;
	
	temp = *next_free;
	*next_free = offset;
	*((size_t*)block - 1) = temp;
}

size_t FSMACountFree(fsma_t *fsma)
{
	size_t count = 0;
	size_t next = 0; 
	
	assert(NULL != fsma);
	
	next = fsma->next_free;
	
	while(0 != next)
	{
		++count;
		next = *((size_t*)fsma + next / WORD_SIZE);
	}
	
	return count;
}


