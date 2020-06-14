/****************************************************
*		Heap Data Structure
*		Author: Yoav Klein
*		Reviewer: Andrei
*		Date: 19.07.19
****************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "heap.h" /* HeapCreate */
#include "dvec.h" /* DvecCreate */

#define WORD_SIZE 8
#define INIT_SIZE 10
#define NUM_CHILDREN 2 

enum side { LEFT = 1, RIGHT };

struct heap
{
	dvec_t *vector;
	int (*compare)(const void *data1, const void *data2, const void *params);
};

static void Swap(heap_t *heap, int parent_index, int child_index);
static void *GetData(const heap_t *heap, int index);
static void HeapifyUp(heap_t *heap, int child_index);
static int LastIndex(heap_t *heap);
static int GetChildIndex(int index, enum side side);
static int GetParentIndex(int index);
static int GetMaxChild(heap_t *heap, int index);
static void HeapifyDown(heap_t *heap, int index);
static void RemoveElement(heap_t *heap, int index);

/* compare - returns 1 if data2 should be before  data1 */
heap_t *HeapCreate(int (*compare)(const void *data1, const void *data2,
                                                   const void *params))
{
	heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}
	
	assert(compare);
	
	heap->vector = DVECCreate(WORD_SIZE, INIT_SIZE);
	if (NULL == heap->vector)
	{
		free(heap);
		
		return NULL;
	}
	heap->compare = compare;
	
	return heap;
}

void HeapDestroy(heap_t *heap)
{
	assert(heap);
	
	DVECDestroy(heap->vector);		

	free(heap);	
}

static void Swap(heap_t *heap, int parent_index, int child_index)
{
	void **parent_data = NULL;
	void **child_data = NULL;
	void *temp = NULL;
	
	parent_data = DVECGetItemAddress(heap->vector, parent_index);
	child_data = DVECGetItemAddress(heap->vector, child_index);
	
	temp = *parent_data;
	*parent_data =*child_data;
	*child_data = temp;
}

static void *GetData(const heap_t *heap, int index)
{
	assert(heap);	
	
	return *(void**)DVECGetItemAddress(heap->vector, index);
}

static void HeapifyUp(heap_t *heap, int child_index)
{
	int parent_index = 0;
	int swap_flag = 0;
	
	void *parent_data = NULL;
	void *child_data = NULL;
	
	parent_index = GetParentIndex(child_index); 
	
	parent_data = GetData(heap, parent_index);
	child_data = GetData(heap, child_index);
	
	swap_flag = heap->compare(parent_data, child_data, NULL); 
	
	while (1 == swap_flag && child_index > NUM_CHILDREN) 
	/* when child_index is one level after root, stop */
	{
		Swap(heap, parent_index, child_index);
		child_index = parent_index;
		parent_index = GetParentIndex(child_index); 
		parent_data = GetData(heap, parent_index);
		swap_flag = heap->compare(parent_data, child_data, NULL); 
	}
	
	/* check if child is larger then root (last result of compare), if is, swap */
	if (1 == swap_flag)
	{
		Swap(heap, parent_index, child_index);
	}
}

int HeapPush(heap_t *heap, void *data)
{
	int status = 0;
	
	assert(heap);
	
	status = DVECPushBack(heap->vector, &data);
	
	if (status)
	{
		return status;
	}
	
	if (1 < HeapSize(heap))
	{
		HeapifyUp(heap, LastIndex(heap));
	}
	
	return status;
}

void *HeapPeek(const heap_t *heap)
{
	assert(heap);
	
	return GetData((heap_t*)heap, 0);		
}

static int LastIndex(heap_t *heap)
{
	return HeapSize(heap) - 1;
}

static int GetChildIndex(int index, enum side side)
{
	return index * NUM_CHILDREN + side;
}

static int GetParentIndex(int index)
{
	return index / NUM_CHILDREN - (index % NUM_CHILDREN == 0);
}

static int GetMaxChild(heap_t *heap, int index)
{
	int right_child_index = 0;
	int left_child_index = 0;
	
	void *left_child_data = NULL;
	void *right_child_data = NULL;
	
	right_child_index = GetChildIndex(index, RIGHT);
	left_child_index = GetChildIndex(index, LEFT);
	
	left_child_data = GetData(heap, left_child_index);	
	
	if (LastIndex(heap) >= index * NUM_CHILDREN + RIGHT) /* if there's right child */
	{
		right_child_data = GetData(heap, right_child_index);
		
		return heap->compare(left_child_data, right_child_data, NULL) ? 
							right_child_index : left_child_index;
	}
	
	return left_child_index;
}

static void HeapifyDown(heap_t *heap, int index)
{
	int swap_flag = 0;
	int max_child_index = 0;
	
	void *max_child_data = NULL;
	void *parent_data = NULL;
	
	max_child_index = GetMaxChild(heap, index);
	max_child_data = GetData(heap, max_child_index);
	parent_data = GetData(heap, index);
	
	swap_flag = heap->compare(parent_data, max_child_data, NULL);
	while (1 == swap_flag)
	{
		Swap(heap, index, max_child_index);
		index = max_child_index;
		if (LastIndex(heap) >= GetChildIndex(index, LEFT))
		{
			max_child_index = GetMaxChild(heap, index);
			max_child_data = GetData(heap, max_child_index);
			swap_flag = heap->compare(parent_data, max_child_data, NULL);	
		}
		else
		{
			swap_flag = 0;
		}
	}
}

static void RemoveElement(heap_t *heap, int index)
{
	Swap(heap, index, LastIndex(heap));
	
	DVECPopBack(heap->vector);
	
	if (LastIndex(heap) >= GetChildIndex(index, LEFT)) /* check if has children */ 
	{
		HeapifyDown(heap, index);	
	}
	
	if(0 != index) /* not the root */
	{
		HeapifyUp(heap, index);
	}
}

void HeapPop(heap_t *heap)
{
	assert(heap);
	
	RemoveElement(heap, 0);
}

int HeapRemove(heap_t *heap, 
               int (*should_remove)(const void *data,
                                   const void *key,
                                   const void *params),
               const void *key)
{
	void *current = NULL;
	size_t i = 0;
	
	assert(heap && should_remove);
	
	for (i = 0; i < HeapSize(heap); ++i)
	{
		current = GetData(heap, i);
		if (1 == should_remove(current, key, NULL))
		{
			RemoveElement(heap, i);
			
			return 0;
		}
	}
	
	return 1;
}

size_t HeapSize(const heap_t *heap)
{
	assert(heap);
	
	return DVECSize(heap->vector);
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	
	return HeapSize(heap) == 0;
}

