/******************************************
*		Priority Queue - Heap based
*		Author: Yoav Klein
*		Reviewer: Andrei
*		Date: 19.7.19
*******************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pq.h" /* PQCreate */
#include "heap.h" /* HeapCreate */

struct pq
{
	heap_t *heap;
};

pq_t *PQCreate(pqpriority_f priority_function)
{
	pq_t *pq = (pq_t*)malloc(sizeof(pq_t));
	if (NULL == pq)
	{
		return NULL;
	}
	
	assert(priority_function);
	
	pq->heap = HeapCreate(priority_function);
	if (NULL == pq->heap)
	{
		free(pq);
		
		return NULL;
	}
	
	return pq;
}

void PQDestroy(pq_t *pq)
{
	assert(pq);
	
	HeapDestroy(pq->heap);
	
	free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
	assert(pq);
	
	return HeapPush(pq->heap, data);
}

void *PQPeek(const pq_t *pq)
{
	assert(pq);
	
	return HeapPeek(pq->heap);
}

int PQDequeue(pq_t *pq)
{
	assert(pq);
	
	HeapPop(pq->heap);
	
	return 0;
}

int PQIsEmpty(const pq_t *pq)
{
	assert(pq);
	
	return HeapIsEmpty(pq->heap);
}

size_t PQSize(const pq_t *pq)
{
	assert(pq);
	
	return HeapSize(pq->heap);
}

void PQClear(pq_t *pq)
{
	assert(pq);

	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

int PQErase(pq_t *pq, const void *key, pqcmp_f should_erase)
{
	assert(pq && should_erase);
	
	return HeapRemove(pq->heap, should_erase, key);
}

