/************************************************************
*			Data Structure: Singular Link List
*			Author: Yoav Klein
*			Reviwer: Roi Kramer
*			Date: 5.6.19
**************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert  */

#include "../include/sll.h" /* SLLCreate */

struct node
{
    void *data;
    struct node *next;
};

struct sll
{
    struct node *head;
    struct node *tail;
};

sll_t *SLLCreate()
{
	sll_t *sll = (sll_t*)malloc(sizeof(sll_t));		
	if(NULL == sll)
	{
		return NULL;
	}
	
	sll->head = (struct node*)malloc(sizeof(struct node));
	sll->tail = (struct node*)malloc(sizeof(struct node));
	
	if(NULL == sll->head || NULL == sll->tail)
	{
		free(sll);
		return NULL;
	}
	
	sll->head->next = sll->tail;
	sll->tail->next = NULL;
	sll->head->data = (void*)0xDEADBEEF;
	sll->tail->data = (void*)0xDEADBEEF;
	
	return sll;
}

static void FreeNodes(it_t start, it_t end)
{
	if(start->next != end)
	{
		FreeNodes(start->next, end);
	}
	free(start);
}

void SLLDestroy(sll_t *sll)
{
	assert(NULL != sll);
	
	FreeNodes(sll->head , sll->tail);
	free(sll->tail);
	free(sll);
}

it_t SLLInsertAfter(sll_t *sll, it_t iter, const void *data)
{
	it_t temp = NULL;
	struct node *new = (struct node*)malloc(sizeof(struct node));
	
	assert(NULL != sll && NULL != iter && NULL != data);
	
	temp = iter->next;
	
	if(NULL == new)
	{
		return sll->tail;
	}
	
	new->data = (void*)data;
	new->next = temp;
	iter->next = new;
	
	return new;
}

void SLLPopBack(sll_t *sll)
{
	struct node *one_before_last = NULL; 
	
	assert(NULL != sll);

	one_before_last = sll->head;

	while(one_before_last->next->next != sll->tail)
	{
		one_before_last = one_before_last->next;
	}
	
	FreeNodes(one_before_last->next, sll->tail);
	one_before_last->next = sll->tail;
}

void SLLPopFront(sll_t *sll)
{
	struct node *temp = NULL;
	
	assert(NULL != sll);
	
	temp = sll->head->next;
	
	sll->head->next = sll->head->next->next;
	FreeNodes(temp, sll->head->next);
}

static it_t OneBefore(sll_t *sll, it_t iter)
{
	it_t curr = NULL;
	
	assert(NULL != sll &&  NULL != iter);
	
	curr = sll->head;
	
	while(curr->next != iter)
	{
		curr = curr->next;
	}
	
	return curr;
}

it_t SLLPushBack(sll_t *sll, const void *data)
{	
	it_t one_before_last = NULL;
		
	assert(NULL != sll && NULL != data);		
	
	one_before_last = OneBefore(sll, sll->tail);
	
	return SLLInsertAfter(sll, one_before_last, data);
}

it_t SLLPushFront(sll_t *sll, const void *data)
{
	return SLLInsertAfter(sll, sll->head, data);
}

void *SLLGetData(it_t iter)
{
	assert(NULL != iter);
	
	return iter->data;
}

it_t SLLBegin(const sll_t *sll)
{
	assert(NULL != sll);
	
	return sll->head->next;
}

it_t SLLEnd(const sll_t *sll)
{
	assert(NULL != sll);

	return sll->tail;
}

it_t SLLNext(it_t iter)
{
	assert(NULL != iter);	
	
	return iter->next;
}

int SLLForEach(it_t from, it_t to, const void *param, act_f action)
{
	it_t i = from;
	int status = 0;
	
	assert(NULL != from && NULL != to && NULL != param && NULL != action);
		
	for(i = from; i != to && 1 != status; i = SLLNext(i))
	{
		status = action(i->data, param);
	}
	
	return status;
}

it_t SLLFind(it_t from, it_t to, void *key, cmp_f compare)
{
	it_t i = from;
	
	assert(NULL != from && NULL != to && NULL != key && NULL != compare);

	for(i = from; i != to; i = SLLNext(i))
	{
		if(compare(i->data, key))
		{
			return i;
		}
	}
	
	return i;
}

int SLLIsEmpty(const sll_t *sll)
{
	assert(NULL != sll);
	
	return (sll->head->next == sll->tail);
}

size_t SLLSize(const sll_t *sll)
{
	size_t count = 0;
	struct node *current = NULL;
	
	assert(NULL != sll);
	
	current = sll->head;
	
	while(current->next != sll->tail)
	{
		++count;
		current = current->next;
	}
	
	return count;
}
 

sll_t *SLLFlip(sll_t *sll)
{
	it_t temp = NULL;
	
	if(SLLBegin(sll)->next != sll->tail)
	{
		temp = SLLBegin(sll);
		sll->head->next = SLLBegin(sll)->next;
		SLLFlip(sll);
		temp->next->next = temp;
		temp->next = sll->tail;
	}
	
	return sll;
}


void SLLErase(sll_t *sll, it_t where)
{
	struct node *one_before = NULL;
	
	assert(NULL != where && NULL != sll);
	
	one_before = OneBefore(sll, where);
	
	one_before->next = where->next;
	FreeNodes(where, where->next);
}


