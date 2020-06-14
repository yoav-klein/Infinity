/****************************************
*		Data Structure: Circular Buffer
*		Author: Yoav Klein
*		Reviewer: Inbar Namdar
*		Date: 3.6.19
*********************************************/

#include <stdlib.h> /* malloc */
#include <errno.h> /* errno */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "../include/cbuf.h"

struct Cbuf
{
	size_t capacity;
	size_t size;
	size_t read_offset;
	void *buff;
};

cbuf_t *CBUFCreate(size_t nbytes)
{ 	
	cbuf_t *cbuf = (cbuf_t*)malloc(sizeof(cbuf_t));
	if(NULL == cbuf)
	{
		return NULL;
	}
	
	assert(0 < nbytes);
	
	cbuf->buff = malloc(nbytes);
	if(NULL == cbuf->buff)
	{
		free(cbuf);
		return NULL;
	}
	
	cbuf->capacity = nbytes;
	cbuf->size = 0;
	cbuf->read_offset = 0;
	
	return cbuf;
}

size_t CBUFWrite(cbuf_t* cbuf, const void *data, size_t nbytes)
{
	int tail = 0;
	char *buff = (char*)cbuf->buff;
	char *data_run = (char*)data;
	
	size_t capacity = cbuf->capacity;
	size_t read_offset = cbuf->read_offset;
	
	assert(NULL != cbuf && NULL != data);
	
	if(cbuf->size + nbytes >= capacity) /* set nbytes if there's an override */
	{
		nbytes = capacity - cbuf->size;
		errno = EOVERFLOW;
	}
	
	/* if there is an overlap, memcpy twice */
	if(cbuf->size + read_offset + nbytes > capacity) 
	{
		tail = capacity - read_offset - cbuf->size;
		memcpy(buff + read_offset + cbuf->size, data_run, tail);
		memcpy(buff, data_run + tail, nbytes - tail);
	}
	else
	{
		memcpy(buff + read_offset + cbuf->size, data_run, nbytes);
	}
	
	cbuf->size += nbytes;
	return nbytes;
}

size_t CBUFRead(cbuf_t* cbuf, void *data, size_t nbytes)
{
	int tail = 0;
	char* buff = (char*)cbuf->buff;
	char *data_run = (char*)data;
	
	/* if trying to read more then size, set nbytes */
	if(nbytes >= cbuf->size)
	{
		nbytes = cbuf->size;
		errno = ENODATA;
	}
	
	/* if overlaping, memcpy twice */
	if(cbuf->read_offset + nbytes > cbuf->capacity)
	{
		tail = cbuf->capacity - cbuf->read_offset;
		memcpy(data_run, buff + cbuf->read_offset, tail);
		memcpy(data_run + tail, buff, nbytes - tail);
	}
	else
	{
		memcpy(data_run, buff + cbuf->read_offset, nbytes);
	}
	
	cbuf->size -= nbytes;
	cbuf->read_offset = (cbuf->read_offset + nbytes) % cbuf->capacity;
	
	return nbytes;
}

size_t CBUFFreeSpace(cbuf_t *cbuf)
{
	assert(NULL != cbuf);
	
	return cbuf->capacity - cbuf->size;
}

int CBUFIsEmpty(const cbuf_t *cbuf)
{
	assert(NULL != cbuf);
	
	return (0 == cbuf->size);
}

size_t CBUFCapacity(cbuf_t *cbuf)
{
	assert(NULL != cbuf);
	
	return cbuf->capacity;
}

void CBUFDestroy(cbuf_t *cbuf)
{
	assert(NULL != cbuf);
	
	free(cbuf->buff);
	free(cbuf);
}




