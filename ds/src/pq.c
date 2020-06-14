/**********************************************
*		Priority Queue
*		Author: Yoav Klein
*		Reviewer: Mishel Liberman
*		Date: 12.6.19
************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/pq.h" /* PQCreate */

struct pq
{
	srtl_t *list;
};

pq_t *PQCreate(pqcmp_f priority_function)
{
	pq_t *pq = (pq_t*)malloc(sizeof(pq_t));
	if(NULL == pq)
	{
		return NULL;
	}
	
	pq->list = SrtLCreate(priority_function, NULL);
	if(NULL == pq->list)
	{
		free(pq);
		
		return NULL;
	}
	
	return pq;
}

void PQDestroy(pq_t *pq)
{
	SrtLDestroy(pq->list);
	free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
	assert(NULL != pq && NULL != data);
	
	return SrtLEnd(pq->list) == SrtLInsert(pq->list, data) ? 1 : 0;
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SrtLGetData(SrtLBegin(pq->list));
}

int PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	
	SrtLPopFront(pq->list);
	return 0;
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SrtLIsEmpty(pq->list);
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SrtLSize(pq->list);
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);
	
	while(!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

int PQErase(pq_t *pq, const void *key, pqcmp_f should_erase)
{
	sit_t current = NULL;
	
	assert(NULL != pq && NULL != key && NULL != should_erase);
	
	current = SrtLBegin(pq->list);
	while(current != SrtLEnd(pq->list) && !should_erase(SrtLGetData(current), key, NULL))
	{
		current = SrtLNext(current);
	}
	
	if(current == SrtLEnd(pq->list))
	{
		return 1;
	}
	
	SrtLErase(current);
	return 0;
}


