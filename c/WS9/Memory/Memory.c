/*********************************************************************
*                  Memory functions
*                  Author: Yoav Klein
*                  Reviwer: Mishel Liberman
*                  Date: 26.5.19
**********************************************************************/

#include <stddef.h> /* size_t */
#include "memory.h" 

void *Memset(void *s, int c, size_t n)
{
	char *run = s;
	size_t *chunk_run = NULL;
	size_t i = 0;
	size_t chunk = 0;
	int head_tail = 0;
	
	for(i = 0; i < WORD_SIZE ; ++i) /* initialize chunk with c */
	{
		*((char*)&chunk + i) = c;
	}
	
	head_tail = (WORD_SIZE - (size_t)run % WORD_SIZE) % WORD_SIZE;
	
	while(head_tail > 0 && n > 0) /* runs until alignment, copying byte by byte */
	{
		*run = c;
		++run;
		--n;
		--head_tail;
	}
	
	head_tail = n % WORD_SIZE;
	n = n / WORD_SIZE;
	chunk_run = (size_t*)run;
	
	for(i = 0; i < n; ++i) /* copying chunks */
	{
		*chunk_run = chunk;
		++chunk_run;
	}
	
	run = (char*)chunk_run;
	
	while(head_tail > 0)  /* copying tail */
	{
		*run = c;
		++run;
		--head_tail;
	}
	
	return s;
}

void *Memcpy(void *dest, void *src, size_t n)
{
	size_t i = 0;
	int head_tail = 0;
	
	size_t *chunk_runner_src = NULL;
	size_t *chunk_runner_dest = NULL;
	char *runner_src = src;
	char *runner_dest = dest;
	
	/* find number of iterations until alignment */
	head_tail = (WORD_SIZE - (size_t)dest % WORD_SIZE) % WORD_SIZE;
	
	while(head_tail > 0 && n > 0) /*copying byte by byte until dest aligns */
	{
		*runner_dest = *runner_src;
		++runner_dest;
		++runner_src;
		--n;
		--head_tail;
	}
	
	/*copying chunks of word size */
	chunk_runner_src = (size_t*)runner_src;
	chunk_runner_dest = (size_t*)runner_dest;
	
	head_tail = n % WORD_SIZE;
	n = n / WORD_SIZE;
	
	for(i = 0; i < n; ++i)
	{
		*chunk_runner_dest = *chunk_runner_src;
		++chunk_runner_dest;
		++chunk_runner_src;
	}
	
	/* byte by byte 'til the last byte */
	runner_dest = (char*)chunk_runner_dest;
	runner_src = (char*)chunk_runner_src;
	
	while(head_tail > 0)
	{
		*runner_dest = *runner_src;
		++runner_dest;
		++runner_src;
		--head_tail;
	}
	
	return dest;
}

void *Memmove(void* dest, const void *src, size_t n)
{
	char *run_dest = dest;
	char *run_src = (void*)src;
	size_t *chunk_run_dest = NULL;
	size_t *chunk_run_src = NULL;
	
	int head_tail = 0;
	int step = 1;
	int flag = 0;
	size_t i = 0;
	
	if(dest > src) /* if dest > src copying will start at the end */
	{
		run_dest += n - 1;
		run_src += n - 1;
		step = -1;
		head_tail = (size_t)run_dest % WORD_SIZE + 1;
	}
	
	else
	{
		head_tail = (WORD_SIZE - ((size_t)run_dest % WORD_SIZE)) % WORD_SIZE; 
	}
	
	while(head_tail > 0 && n > 0)  /* copying until alignment */
	{
		*run_dest = *run_src;
		run_dest += step;
		run_src += step;
		--n;
		--head_tail;
	}
	
	/* if dest > src and there are chunks to copy, positioning the pointers at right place */
	if(dest > src && n >= WORD_SIZE) 
	{
		run_dest -= (WORD_SIZE - 1);
		run_src -= (WORD_SIZE - 1);
		flag = 1;
	}
	
	chunk_run_dest = (size_t*)run_dest;
	chunk_run_src = (size_t*)run_src;
	
	head_tail = n % WORD_SIZE;
	n = n / WORD_SIZE;
	
	for(i = 0; i < n; ++i) /* copying chunks */
	{
		*chunk_run_dest = *chunk_run_src;
		chunk_run_dest += step;
		chunk_run_src += step;
	}
	
	run_dest = (char*)chunk_run_dest;
	run_src = (char*)chunk_run_src;
	
	if(1 == flag) /* if there were chunks copied, positioning pointers at right spots */
	{
		run_dest += WORD_SIZE - 1;
		run_src += WORD_SIZE - 1;
	}
	
	while(head_tail > 0)
	{
		*run_dest = *run_src;
		run_dest += step;
		run_src += step;
		--head_tail;
	}
	
	return dest;
}





