#ifndef FSMA_H
#define FSMA_H

#include <stddef.h>

typedef struct fsma fsma_t;

/* the function receives as parameters the desired number
of blocks n_blocks and block size, and returns the number
of bytes to be allocated by the user in order for valid
sevice of the next functions in the library.
if the user chooses to allocate less space, behaviour is undefined */
size_t FSMASuggestSize(size_t n_blocks, size_t block_size);

/* the function receives as parameters a pointer to the allocated 
space pool, the number of bytes allocates pool_size and the block 
size desired by the user block_size. 
the function returns fsma_t pointer that should be defined by the user */
fsma_t *FSMAInit(void *pool, size_t pool_size, size_t block_size);

/* the function allocates a block of bytes in the size that was
defined by the user. receives fsma_t pointer as a parameter and
returns a pointer to the allocated space.
it's the user's responsibility to check if there is free space
to be used, otherwise behaviour is undefined. */
void *FSMAAlloc(fsma_t *fsma);

/* the function frees a block of memory for use. receives the desired block 
to be freed. 
it's the user's responsibility not to free a memory space that was
freed before or wasn't allocated. */
void FSMAFree(void *block);

/* receives a fsma_t pointer and returns the number of 
free blocks in the pool */
size_t FSMACountFree(fsma_t *fsma);

#endif
