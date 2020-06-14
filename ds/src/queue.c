/***************************************************************
*			Data Structure: Queue
*			Author: Yoav Klein
*			Reviewer: Eldad Carin
*			Date: 6.6.19
**************************************************************/

#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "../include/queue.h" /* QueueCreate */
#include "../include/sll.h"   /*  SLLCreate  */

struct queue
{
	sll_t *list;
};

queue_t *QueueCreate()
{
	queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
	if(NULL == queue)
	{
		return NULL;
	}
	
	queue->list = SLLCreate();
	if(NULL == queue->list)
	{
		return NULL;
	}
	
	return queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SLLDestroy(queue->list);
	free(queue);
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return SLLIsEmpty(queue->list);
}

int QueueEnqueue(queue_t *queue, const void *data)
{
	it_t iter = NULL;

	assert(NULL != queue);
	assert(NULL != data);
	
	iter = SLLPushBack(queue->list, data);

	return (iter == SLLEnd(queue->list) ? 1 : 0);

}

void QueueDequeue(queue_t *queue)
{
	assert(NULL != queue);
	
	SLLPopFront(queue->list);
}

void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);	
	
	return SLLGetData(SLLBegin(queue->list));
}

size_t QueueSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return SLLSize(queue->list);
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	it_t i = NULL;
	
	for(i = SLLBegin(src->list); i != SLLEnd(src->list); i = SLLNext(i))
	{
		QueueEnqueue(dest, SLLGetData(i));
	}
	
	return dest;
	
}

