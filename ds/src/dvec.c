
/*************************************************
*		Data Structure: Dynamic Vector
*		Author: Yoav Klein
*		Reviewer: 
*		Date: 3.6.19
***************************************************/

#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "../include/dvec.h"

struct Dvec
{
	void *arr;
	size_t size_of_element;
	size_t num_of_elements;
	size_t capacity;
};

dvec_t *DVECCreate(size_t size_of_element, size_t num_elements)
{
	void *arr = NULL; 
	dvec_t *dvec = (dvec_t*)malloc(sizeof(dvec_t));
	if(NULL == dvec)
	{
		return NULL;
	}
	
	assert(0 != size_of_element);
	
	dvec->capacity = num_elements;
	
	arr = malloc(size_of_element * dvec->capacity);
	if(NULL == arr)
	{
		free(dvec);
		return NULL;
	}
	
	dvec->arr=arr;
	dvec->size_of_element = size_of_element;
	dvec->num_of_elements = 0;
	
	return dvec;
}

void DVECDestroy(dvec_t *dvec)
{
	assert(NULL != dvec);
	
	free(dvec->arr);
	free(dvec);
}

void *DVECGetItemAddress(dvec_t *dvec, size_t index)
{
	assert(NULL != dvec);
	
	if(dvec->num_of_elements >= index + 1)
	{ 
		return ((char*)dvec->arr + dvec->size_of_element * index);
	}
	
	return NULL;
}

int DVECPushBack(dvec_t *dvec, const void *element)
{
	void *dest = NULL;
	void *temp = NULL;
	
	assert(NULL != dvec && NULL != element);
	
	dest = (char*)dvec->arr + (dvec->size_of_element * dvec->num_of_elements);
	
	memcpy(dest, element, dvec->size_of_element);
	++(dvec->num_of_elements);
	
	if(dvec->num_of_elements >= dvec->capacity)
	{
		temp = realloc(dvec->arr, (dvec->capacity * 2) * dvec->size_of_element);
		if(NULL == temp)
		{
			return 1;
		} 
		dvec->arr = temp;
		dvec->capacity *= 2;
	}
	
	return 0;
}

void DVECPopBack(dvec_t *dvec)
{
	void *temp = NULL;
	
	assert(NULL != dvec);
	
	if(dvec->num_of_elements == 0)
	{
		return;	
	}
	
	--dvec->num_of_elements;
	 
	if(dvec->num_of_elements <= dvec->capacity / 4)
	{
		temp = realloc(dvec->arr, (dvec->capacity / 2) * dvec->size_of_element);
		if(NULL == temp)
		{
			return;
		}
		dvec->arr = temp;
		dvec->capacity /= 2;
	}
}

size_t DVECSize(const dvec_t *dvec)
{
	assert(NULL != dvec);
	
	return dvec->num_of_elements;	
}

size_t DVECCapacity(const dvec_t *dvec)
{
	assert(NULL != dvec);
	
	return dvec->capacity;
}

int DVECReserve(dvec_t *dvec, size_t new_capacity)
{
	void *temp = NULL;
	assert(NULL != dvec && new_capacity > 0);

	if(dvec->num_of_elements >= new_capacity)
	{
		new_capacity = dvec->num_of_elements + 1;		
	}
	
	temp = realloc(dvec->arr, new_capacity * dvec->size_of_element);
	if(NULL == temp)
	{
		return 1;
	}
	
	dvec->arr = temp;
	dvec->capacity = new_capacity;
	
	return 0;
}

