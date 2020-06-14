/************************************************
*		Hash Table
*		Author: Yoav Klein
*		Reviewer: Eldad Carin
*		Date: 16.7.19	
***************************************************/

#include <assert.h> /* assert */
#include <math.h> /* pow */
#include <stdlib.h> /* malloc */

#include "hash.h" /* HashCreate */
#include "dll.h" /* DLLCreate */

struct hash
{
	int (*compare)(const void *data, const void *key);
	size_t (*hash_f)(void *key);
	size_t range;
	dll_t **table; 
};

int CMPWrapper(const void *data, const void *key, const void *params)
{
	hash_t *ht = (hash_t*)params;
	
	return ht->compare(data, key);
}

static dll_t *CreateDLLs(dll_t **dlls, size_t range)
{
	dll_t *new = DLLCreate();
	dll_t *next = NULL;
	
	if (NULL == new)
	{
		return NULL;
	}
	
	*dlls = new;
	
	range -= 1;
	
	if (range > 0)
	{
		next = CreateDLLs(dlls + 1, range);
		if (NULL == next)
		{
			DLLDestroy(new);
			
			return NULL;
		}
	}
	
	return new; 
}

hash_t *HashCreate(int (*compare)(const void *data, const void *key),
					size_t (*hash_function)(void *key),
					size_t range)
{
	hash_t *ht = (hash_t*)malloc(sizeof(hash_t));
	if (NULL == ht)
	{
		return NULL;
	}
	
	assert(compare && hash_function);
	
	ht->hash_f = hash_function;
	ht->compare = compare;
	ht->range = range;
	
	ht->table = (dll_t**)malloc(sizeof (dll_t*) * range);
	if (NULL == ht->table)
	{
		free(ht);
		
		return NULL;
	}
	
	*(ht->table) = CreateDLLs(ht->table, range);
	
	return ht;
}

void HashDestroy(hash_t *ht)
{
	size_t i = 0;
	
	assert(ht);
	
	for (i = 0; i < ht->range; ++i)
	{
		DLLDestroy(ht->table[i]);
	}
	
	free(ht->table);
	free(ht);
}

int HashInsert(hash_t *ht, void *data)
{
	size_t entry = 0;
	
	assert(ht);
	
	entry = ht->hash_f(data);
	
	entry = entry % ht->range;
	
	if (DLLEnd(ht->table[entry]) == DLLPushFront(ht->table[entry], data))
	{
		return 1;
	}
	
	return 0;
}

void HashRemove(hash_t *ht, void *key)
{
	it_t iterator = NULL;
	size_t entry = 0;
	dll_t *list = NULL;
	
	assert(ht && key);
	
	entry = ht->hash_f(key);
	entry %= ht->range;
	
	list = ht->table[entry];
	
	iterator = DLLFind(DLLBegin(list), DLLEnd(list), CMPWrapper, key, ht);
	
	if (DLLEnd(list) != iterator)
	{
		DLLErase(iterator); 
	}
}

void *HashFind(const hash_t *ht, void *key)
{
	it_t iterator = NULL;
	size_t entry = 0;
	dll_t *list = NULL;
	
	assert(ht && key);
	
	entry = ht->hash_f(key);
	entry %= ht->range;
	
	list = ht->table[entry];
	
	iterator = DLLFind(DLLBegin(list), DLLEnd(list), CMPWrapper, key, ht);
	
	if (DLLEnd(list) != iterator)
	{
		return DLLGetData(iterator);
	}
	
	return NULL;
}

size_t HashSize(const hash_t *ht)
{
	size_t i = 0;
	size_t count = 0;
	
	assert(ht);
	
	for (i = 0; i < ht->range; ++i)
	{
		count += DLLSize(ht->table[i]);
	}
	
	return count;
}

int HashIsEmpty(const hash_t *ht)
{
	size_t i = 0;
	
	assert(ht);
	
	for (i = 0; i < ht->range; ++i)
	{
		if (!DLLIsEmpty(ht->table[i]))
		{
			return 0;
		}
	}
	
	return 1;
}

int HashForEach(hash_t *ht, 
				int (*act_f)(void *data, const void *params),
				const void *params)
{
	size_t i = 0;
	dll_t *list;
	int status = 0;
	
	assert(ht && act_f);
	
	for (i = 0; i < ht->range; ++i)
	{
		list = ht->table[i];
		status = DLLForEach(DLLBegin(list), DLLEnd(list), act_f, (void*)params);
		if (status != 0)
		{
			return status;
		}
	}
	
	return 0;
}

double HashLoad(hash_t *ht)
{
	assert(ht);

	return (double)HashSize(ht) / ht->range;
}

double HashSD(hash_t *ht)
{
	size_t i = 0;
	double avg = 0;
	double sum = 0;
	double std = 0;
	
	assert(ht);
	
	avg = HashLoad(ht);
	for (i = 0; i < ht->range; ++i)
	{
		sum = sum + pow(DLLSize(ht->table[i]) - avg, 2);
	}
	
	std = sum / ht->range;
	
	return sqrt(std);
}


