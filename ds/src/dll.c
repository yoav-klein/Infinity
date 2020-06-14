/********************************************
*		Data Structure: Doubly Linked List
*		Author: Yoav Klein
*		Reviewer: Andrei the first
*		Date: 6.6.19
***********************************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/dll.h" /* DLLCreate  */

struct dll_node               
{
	void *data;
	struct dll_node *next;
	struct dll_node *prev;
};

struct dll
{
	struct dll_node *head;
	struct dll_node *tail;
};

static it_t CreateNode(const void *data)
{
	it_t node = (it_t)malloc(sizeof(struct dll_node));
	if(NULL == node)
	{
		return NULL;
	}
	
	node->data = (void*)data;
	
	return node;
}

dll_t *DLLCreate()
{
	dll_t* dll = (dll_t*)malloc(sizeof(dll_t));
	if(NULL == dll)
	{
		return NULL;
	}
	
	dll->head = CreateNode((void*)0xDEADBEEF);
	dll->tail = CreateNode((void*)0xDEADBEEF);
	if(NULL == dll->head || NULL == dll->tail)
	{
		free(dll);
		return NULL;
	}
	
	dll->head->next = dll->tail;
	dll->head->prev = NULL;
	
	dll->tail->next = NULL;
	dll->tail->prev = dll->head;
	
	return dll;
}

static void DeleteNode(it_t node)
{
	assert(NULL != node);
	
	node->next->prev = DLLPrev(node);
	node->prev->next = DLLNext(node);
	
	free(node);
}

static void ConnectNode(it_t new, it_t prev_node)
{
	new->prev = prev_node;
	new->next = DLLNext(prev_node);
	prev_node->next = new;
	new->next->prev = new;
}

void DLLDestroy(dll_t *dll)
{
	it_t temp = NULL;
	it_t i = DLLBegin(dll);
	
	assert(NULL != dll);
	
	while(i != DLLEnd(dll))
	{
		temp = DLLNext(i);
		DeleteNode(i);
		i = temp;
	}
	
	free(dll->head);
	free(dll->tail);
	free(dll);
}

it_t DLLBegin(const dll_t *dll)
{
	assert(NULL != dll);

	return dll->head->next;
}

it_t DLLEnd(const dll_t *dll)
{
	assert(NULL != dll);

	return dll->tail;
}

it_t DLLNext(const it_t iter)
{
	assert(NULL != iter);	
	
	return iter->next;
}

it_t DLLPrev(const it_t iter)
{
	assert(NULL != iter);
	
	return iter->prev;
}

size_t DLLSize(const dll_t *dll)
{
	it_t i = NULL;
	size_t count = 0;
	
	assert(NULL != dll);
	
	i = DLLBegin(dll);
	while(i != DLLEnd(dll))
	{
		++count;
		i = DLLNext(i);
	}
	
	return count;
}

int DLLIsEmpty(const dll_t *dll)
{
	assert(NULL != dll);	
	
	return (DLLBegin(dll) == DLLEnd(dll));
}

int DLLIsSameIter(const it_t iter1,const it_t iter2)
{
	assert(NULL != iter1 && NULL != iter2);
	
	return (iter1 == iter2);
}

void *DLLGetData(const it_t iter)
{
	assert(NULL != iter);
	
	return iter->data;
}

it_t DLLInsert(dll_t *dll, it_t iter, void *data)
{
	it_t new = NULL; 
	
	assert(NULL != dll && NULL != iter && NULL != data);
	
	new = CreateNode(data);
	if(NULL == new)
	{
		return DLLEnd(dll);
	}
	
	ConnectNode(new, iter);
	
	return new;
}

it_t DLLErase(it_t where)
{
	it_t temp = NULL;
	
	assert(NULL != where);
	
	temp = DLLNext(where);
	DeleteNode(where);
	
	return temp;
}

it_t DLLPushBack(dll_t *dll, void *data)
{
	assert(NULL != dll && NULL != data);
	
	return DLLInsert(dll, DLLPrev(DLLEnd(dll)), data);
}

it_t DLLPushFront(dll_t *dll, void *data)
{
	assert(NULL != dll && NULL != data);
	
	return DLLInsert(dll, DLLPrev(DLLBegin(dll)), data);
}

void DLLPopBack(dll_t *dll)
{
	assert(NULL != dll);
	
	DeleteNode(DLLPrev(DLLEnd(dll)));
}

void DLLPopFront(dll_t *dll)
{
	assert(NULL != dll);
	
	DeleteNode(DLLBegin(dll));
}

it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params)
{	
	assert(NULL != from && NULL != to && NULL != compare);

	while(from != to)
	{
		if(1 == compare(DLLGetData(from), key, params))
		{
			return from;
		}
		
		from = DLLNext(from);
	}
	
	return to;
}

int DLLForEach(it_t from, it_t to, act_f action, void *params)
{
	int status = 0;
	
	assert(NULL != from && NULL != to && NULL != action);

	while(from != to)
	{
		status = action(DLLGetData(from), params);
		if(status != 0)
		{
			return status;
		}
		
		from = DLLNext(from);
	}
	
	return status;
}

it_t DLLSpliceBefore(it_t where, it_t from, it_t to)
{
	it_t temp = NULL;
	
	assert(NULL != where && NULL != from && NULL != to);
	
	temp = DLLPrev(where);
	to->prev->next = where;
	where->prev = DLLPrev(to);
	from->prev->next = to;
	to->prev = DLLPrev(from);
	from->prev = temp;
	temp->next = from;
	
	return from;
}




