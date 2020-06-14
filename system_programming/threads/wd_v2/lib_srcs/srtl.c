/**********************************************
*			Data Structure: Sorted List
*			Author: Yoav Klein
*			Reviewer: Ben Izakson
*			Date: 11.6.19
************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/srtl.h" /* SrtLCreate */


struct srtl
{
    dll_t *list;
    isbefore_f is_before;
    void *params;
};

srtl_t *SrtLCreate(isbefore_f is_before, const void *params)
{
	srtl_t *srtl = (srtl_t*)malloc(sizeof(srtl_t));
	if(NULL == srtl || NULL == is_before)
	{
		return NULL;
	}
	
	srtl->list = DLLCreate();
	if(NULL == srtl->list)
	{
		free(srtl);
		return NULL;
	}
	
	srtl->is_before = is_before;
	srtl->params = (void*)params;
	
	return srtl;
}

void SrtLDestroy(srtl_t *srtl)
{
	assert(NULL != srtl);
	
	DLLDestroy(srtl->list);
	free(srtl);
}

size_t SrtLSize(const srtl_t *srtl)
{
	assert(NULL != srtl);	
	
	return DLLSize(srtl->list);
}

int SrtLIsEmpty(const srtl_t *srtl)
{
	assert(NULL != srtl);
	
	return DLLIsEmpty(srtl->list);
}

sit_t SrtLInsert(srtl_t *srtl, void *data)
{
	sit_t current = NULL;
	
	assert(NULL != srtl && NULL != data);
	
	current = DLLBegin(srtl->list);
	while(current != SrtLEnd(srtl) && !srtl->is_before(SrtLGetData(current),
	data, srtl->params))
	{
		current = SrtLNext(current);
	}
	
	return DLLInsert(srtl->list, SrtLPrev(current), data);
}

sit_t SrtLErase(sit_t where)
{
	assert(NULL != where);
	
	return DLLErase(where);
}

void SrtLPopFront(srtl_t *srtl)
{
	assert(NULL != srtl);
	
	DLLPopFront(srtl->list);
}

void SrtLPopBack(srtl_t *srtl)
{
	assert(NULL != srtl);

	DLLPopBack(srtl->list);
}

sit_t SrtLFind(sit_t from, sit_t to, scmp_f compare, const void *key, 
const void *params)
{
	it_t current = NULL;
	
	assert(NULL != from && NULL != to && NULL != compare && NULL != key);
	
	current = from;
	
	for(; current != to && compare(SrtLGetData(current), key, params) == 0;
	current = SrtLNext(current));
	
	return current;
}

int SrtLForEach(sit_t from, sit_t to, sact_f action, const void *params)
{
	assert(NULL != from && NULL != to && NULL != action && NULL != params);
	
	return DLLForEach(from, to, action, (void*)params);  
}

sit_t SrtLBegin(const srtl_t *srtl)
{
	assert(NULL != srtl);
	
	return DLLBegin(srtl->list);
}

sit_t SrtLEnd(const srtl_t *srtl)
{
	assert(NULL != srtl);
	
	return(DLLEnd(srtl->list));
}

sit_t SrtLNext(const sit_t iter) /* do not pass SrtLEnd ("end of srtl") as 'iter' */
{
	assert(NULL != iter);
	
	return(DLLNext(iter));
}

sit_t SrtLPrev(const sit_t iter) /* do not pass SrtLEnd ("end of srtl") as 'iter' */
{
	assert(NULL != iter);
	
	return DLLPrev(iter);
}

void *SrtLGetData(const sit_t iter)
{
	assert(NULL != iter);	
	
	return DLLGetData(iter);
}

int SrtLIsSame(const sit_t iter1, const sit_t iter2)
{
	assert(NULL != iter1 && NULL != iter2);
	
	return iter1 == iter2;
}

void SrtLMerge(srtl_t *dest, srtl_t *src)
{
	sit_t where = SrtLBegin(dest);
	sit_t from = SrtLBegin(src);
	sit_t to = from;
	
	while(to != SrtLEnd(src))
	{
		while(where != SrtLEnd(dest) && 
		!dest->is_before(SrtLGetData(where), SrtLGetData(from), dest->params))
		{
			where = SrtLNext(where);
		}
		
		if(where == SrtLEnd(dest))
		{
			to = SrtLEnd(src);
		}
		
		while(to != SrtLEnd(src) && 
		!dest->is_before(SrtLGetData(to), SrtLGetData(where), dest->params))
		{
			to = SrtLNext(to);
		}
		
		DLLSpliceBefore(where, from, to);
		
		from = to;
	}
}




