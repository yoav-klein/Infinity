
#include <stdio.h>
#include <stdlib.h>

#include "fsma.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }
								


int FlowTest()
{
	struct test 
	{
		size_t a;
		size_t b;
		size_t c;
	};
	
	int flag = 1;
	size_t res = 0;
	void *pool;
	size_t suggested = 0;
	fsma_t *fsma;
	
	struct test *first;
	struct test *second;
	struct test *third;
	struct test *fourth;
	
	suggested = FSMASuggestSize(4, sizeof(struct test));
	
	/* 24 for the struct, (24 + 8) * 4 */
	if(152 != suggested)
	{
		flag = 0;
	}
	
	pool = malloc(suggested);
	
	fsma = FSMAInit(pool, suggested, 24);
	
	/* check size, should be 4) */
	res = FSMACountFree(fsma);
	if(4 != res)
	{
		flag = 0;
	}
	
	first = (struct test*)FSMAAlloc(fsma);
	res = FSMACountFree(fsma);
	if(3 != res)
	{
		flag = 0;
	}
	
	second = (struct test*)FSMAAlloc(fsma);
	res = FSMACountFree(fsma);
	if(2 != res)
	{
		flag = 0;
	}
	
	third = (struct test*)FSMAAlloc(fsma);
	res = FSMACountFree(fsma);
	if(1 != res)
	{
		flag = 0;
	}
	
	fourth = (struct test*)FSMAAlloc(fsma);
	res = FSMACountFree(fsma);
	if(0 != res)
	{
		flag = 0;
	}
	
	first->a = 1;
	first->b = 2;
	first->c = 3;
	
	second->a = 10;
	second->b = 20;
	second->c = 30;
	
	third->a = 100;
	third->b = 200;
	third->c = 300;
	
	fourth->a = 1000;
	fourth->b = 2000;
	fourth->c = 3000;
	
	/* Free and check size and contents */
	FSMAFree(fourth);
	
	res = FSMACountFree(fsma);
	if(1 != res)
	{
		flag = 0;
	}
	
	if(100 != third->a)
	{
		flag = 0;
	}
	
	if(300 != third->c)
	{
		flag = 0;
	}
	
	if(1 != first->a)
	{
		flag = 0;
	}
	
	if(20 != second->b)
	{
		flag = 0;
	}
	
	/* Free and check size and contents */
	FSMAFree(first);
	res = FSMACountFree(fsma);
	if(2 != res)
	{
		flag = 0;
	}
	
	if(100 != third->a)
	{
		flag = 0;
	}
	
	if(300 != third->c)
	{
		flag = 0;
	}
	
	if(20 != second->b)
	{
		flag = 0;
	}
	
	/* alloc again, check size */
	first = FSMAAlloc(fsma);
	
	first->a = 1;
	first->b = 2;
	first->c = 3;
	
	res = FSMACountFree(fsma);
	if(1 != res)
	{
		flag = 0;
	}
	
	fourth = FSMAAlloc(fsma);
	fourth->a = 1000;
	fourth->b = 2000;
	fourth->c = 3000;
	
	res = FSMACountFree(fsma);
	if(0 != res)
	{
		flag = 0;
	}
	
	/* free the second, check contents */
	FSMAFree(second);
	
	res = FSMACountFree(fsma);
	if(1 != res)
	{
		flag = 0;
	}
	
	if(100 != third->a)
	{
		flag = 0;
	}
	
	if(3 != first->c)
	{
		flag = 0;
	}
	
	return flag;
	
}		

int HalfSizes()
{
	struct test 
	{
		size_t a;
		size_t b;
		size_t c;
	};
	
	int flag = 1;
	size_t res = 0;
	void *pool;
	fsma_t *fsma;
	
	struct test *first;
	
	
	/* Allocate less then needed for 4 blocks, 150 bytes for blocks of 24
	bytes. */
	pool = malloc(150);
	
	fsma = FSMAInit(pool, 150, 24);
	
	res = FSMACountFree(fsma);
	if(3 != res)
	{
		flag = 0;
	}
	
	free(pool);
	
	pool = malloc(70);
	
	fsma = FSMAInit(pool, 70, 24);
	
	res = FSMACountFree(fsma);
	if(1 != res)
	{
		flag = 0;
	}
	
	first = FSMAAlloc(fsma);
	res = FSMACountFree(fsma);
	if(0 != res)
	{
		flag = 0;
	}
	
	free(pool);
	
	return flag;
}	
	
int main()
{
	RUN_TEST(FlowTest)
	RUN_TEST(HalfSizes)
	
	return 1;
}
