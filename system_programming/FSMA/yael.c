/************************************************
			Author: Yael Lotan
			Reviewer: Yoav Klein 
************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "fsma.h" /* fsma header file */

#define WORD (sizeof(size_t))
#define STRUCT_SIZE (3 * sizeof(size_t))
#define PADDING(X) ((8 - (X % 8)) % 8)
#define STEP(X) (X + PADDING(X) + WORD)

typedef struct metadata metadata_t;

struct fsma
{
	size_t next_free;
	size_t block_size;
	size_t n_blocks;
};

struct metadata
{
    size_t offset;
};

size_t FSMASuggestSize(size_t n_blocks, size_t block_size)
{
    size_t suggusted_cap = 0;
    suggusted_cap = n_blocks * STEP(block_size) + STRUCT_SIZE;
   
    return suggusted_cap;
}

fsma_t *FSMAInit(void *pool, size_t pool_size, size_t block_size)
{
    size_t runner = STRUCT_SIZE;
    
    fsma_t *fsma = (fsma_t *)pool;
    metadata_t *cur_meta = (metadata_t *)pool + STRUCT_SIZE;
    
    fsma->block_size = block_size;
    fsma->n_blocks = (pool_size - STRUCT_SIZE) / (STEP(block_size));
    fsma->next_free = STRUCT_SIZE;
    cur_meta->offset = STRUCT_SIZE + STEP(block_size);
    
    while (runner < pool_size)
    {
        *((char *)pool + runner) = cur_meta->offset;
        cur_meta->offset += STEP(block_size);
        runner += STEP(block_size);
    }

    return pool;
}

void *FSMAAlloc(fsma_t *fsma)
{
    void *block = fsma->next_free + (char *)fsma;
    
    fsma->next_free = ((metadata_t *)((char *)block))->offset;
    
    *(size_t *)block = (size_t)fsma; 
    
    return (void *)((size_t)block + WORD);
}

void FSMAFree(void *block)
{
	size_t offset_of_block;
	fsma_t *fsma = NULL;
  	fsma = (fsma_t *)(((metadata_t *)((size_t)block - WORD))->offset);
    offset_of_block = (size_t)block - WORD - (size_t)fsma;
  
    *((size_t *)block - WORD) = fsma->next_free;
    fsma->next_free = offset_of_block;
}

size_t FSMACountFree(fsma_t *fsma)
{
    size_t pool_size = (fsma->n_blocks) * (STEP(fsma->block_size))  + STRUCT_SIZE;
    size_t next_free = fsma->next_free;
    size_t counter = 0;
    
    while (next_free < pool_size)
    {
        ++counter;
        next_free = *(size_t *)((char *)fsma + next_free);
    }
    
    return counter;
}
