/************************************************************
*			Bit Array data structure
*			Author: Yoav Klein
*			Reviewer: Andrei Plakhota
*			Date: 28.5.19
***************************************************************/

#include <stdio.h>
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "../include/barr.h"

#define ARR_LEN (sizeof(size_t) * 8)
#define ASCI_SIZE 256

size_t BARRSetBit(size_t arr, int index, int value)
{
	return ((value << index) | (arr & (~((size_t)1 << index))));
}

size_t BARRSetOn(size_t arr, int index)
{
	return (arr | (size_t)1 << index);
}

size_t BARRSetOff(size_t arr, int index)
{
		return (arr & (~((size_t)1 << index)));
}

size_t BARRFlip(size_t arr, int index)
{
	return (arr ^ ((size_t)1 << index));
}

size_t BARRMirror(size_t arr)
{
	size_t i = 0; 
	size_t res = 0;
	
	for(i = 0; i < ARR_LEN / 2; ++i)
	{
		res = res | (arr & (0x8000000000000000 >> i)) >> (ARR_LEN - 1 - (i * 2)) 
		| (arr & ((size_t)1 << i)) << (ARR_LEN - 1 - (i * 2));
	}
	
	return res;
}

static unsigned char *MirrorLUTInit()
{
	int i = 0;
	unsigned char curr = 0;
	unsigned char *LUT = NULL;
	
	LUT = (unsigned char*)malloc(ASCI_SIZE * sizeof(char));
	if(NULL == LUT)
	{
		printf("Couldn't allocate memory\n");
		return NULL;
	}
	
	for(i = 0; i < ASCI_SIZE; ++i)
	{
		curr = i;
		curr = curr << 4 | curr >> 4;
		curr = (curr & 0x33) << 2 | (curr & 0xcc) >> 2;
		curr = (curr & 0x55) << 1 | (curr & 0xaa) >> 1;
		*(LUT + i) = curr;
	}
		
	return LUT;
}

size_t BARRMirrorLUT(size_t arr)
{
	static unsigned char *LUT = NULL;

	int i = 0;
	size_t res = 0;
	
	if(NULL == LUT)
	{
		LUT = MirrorLUTInit(LUT);
	}
	
	if(NULL != LUT)
	{
		for(i = 7; i >= 0; --i)
		{
			res = res | (size_t)(*(LUT + (arr & 0xff))) << i * 8;
			arr = arr >> 8;
		}
	}
	return res;
}

size_t BARRRotateLeft(size_t arr, size_t amount)
{
	assert(amount > 0);
	
	return ((arr << (amount % ARR_LEN)) | (arr >> (ARR_LEN - (amount % ARR_LEN))));
	return arr;
}

size_t BARRRotateRight(size_t arr, size_t amount)
{
	assert(amount > 0);
	
	return ((arr >> (amount % ARR_LEN)) | (arr << (ARR_LEN - (amount % ARR_LEN))));
	return arr;
}

int BARRIsOn(size_t arr, int index)
{
	return ((((size_t)1 << index) & arr) > 0);
}

int BARRIsOff(size_t arr, int index)
{
	return ((((size_t)1 << index) & arr) == 0);
}

size_t BARRCountOn(size_t arr)
{
	arr = (arr & 0x5555555555555555) + ((arr >> 1) & 0x5555555555555555);
	arr = (arr & 0x3333333333333333) + ((arr >> 2) & 0x3333333333333333);
	arr = (arr & 0x0f0f0f0f0f0f0f0f) + ((arr >> 4) & 0x0f0f0f0f0f0f0f0f);
	arr = (arr & 0x00ff00ff00ff00ff) + ((arr >> 8) & 0x00ff00ff00ff00ff);
	arr = (arr & 0x0000ffff0000ffff) + ((arr >> 16) & 0x0000ffff0000ffff);
	arr = (arr & 0x00000000ffffffff) + ((arr >> 32) & 0x00000000ffffffff);
	
	return arr;
}

static int *CountLUTInit()
{
	int i = 0, j = 0;
	int count = 0;
	int *LUT = (int*)malloc(ASCI_SIZE * sizeof(int));
	
	if(NULL == LUT)
	{
		printf("Couldn't allocate memory\n");
		return NULL;
	}
	
	for(i = 0; i < ASCI_SIZE; ++i)
	{
		count = 0;
		for(j = 0; j < 8; ++j)
		{
			if((1 << j) & i)
			{
				++count;
			}
		}
		LUT[i] = count;
	}
	
	return LUT;
}

size_t BARRCountOnLUT(size_t arr)
{
	static int *LUT = NULL;
	
	int i = 0;
	int count = 0;
	
	if(NULL == LUT)
	{
		LUT = CountLUTInit();
	}
	
	if(NULL != LUT)
	{
		for(i = 0; i < 8; ++i)
		{
			count += *(LUT + (int)*((unsigned char*)&arr + i));
		}
	}
	return count;
}

size_t BARRCountOff(size_t arr)
{
	return (ARR_LEN - BARRCountOn(arr));
}


