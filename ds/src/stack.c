/****************************************************
*			ADT - Stack
*			Author: Yoav Klein
*			Reviewer: Inbar Namdar
*			Date: 29.5.19
*********************************************************/

#include <stdio.h> /* printf */ 
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "../include/stack.h"

struct stack
{
	void *current;
	void *end;
	size_t element_size;
	char head[1];
};

stack_t *STACKCreate(size_t n_elements, size_t size_of_element)
{
	stack_t *stack = (stack_t*)malloc(sizeof(stack_t) + n_elements * size_of_element);
	
	assert(n_elements > 0 && size_of_element > 0);
	if(NULL == stack)
	{
		printf("Couldn't allocate memory\n");
		return NULL;
	}
	
	stack->current = stack->head;
	stack->end = (char*)(stack -> current) + n_elements * size_of_element;
	stack->element_size = size_of_element;
	
	return stack;
}

int STACKPush(stack_t *stack, const void* element)
{
	assert(NULL != stack && NULL != element);
	
	if(stack->end > stack->current)
	{
		memcpy(stack->current, element, stack->element_size);
		stack->current =  (char*)stack->current + stack->element_size;
		
		return 0;
	}
	
	return 1;
}

void *STACKPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	if(stack->current != (void*)stack->head)
	{
		return ((void*)((char*)stack->current - stack->element_size));
	}
	
	return NULL;
}

void STACKPop(stack_t *stack)
{
	if((char*)stack->current > (char*)stack->head)
	{
		stack->current = (char*)stack->current - stack->element_size;
	}
}

size_t STACKSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (((size_t)stack->current - (size_t)stack->head) / stack->element_size);
}

void STACKDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack);
}


