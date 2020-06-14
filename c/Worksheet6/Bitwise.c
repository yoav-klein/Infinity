/******************************************************************************************************
*																									   *
*								Worksheet 6 - Bitwise operations									   *
*									Author: Yoav Klein												   *
*									Reviewer: Ben Izakson											   *
*									Date: 20.5.19													   *
*																									   *
*******************************************************************************************************/



#include <stdio.h> /* printf */

#define uint unsigned int
#define INTLEN 32

int LUT[256];

int Pow2(uint x, int y)
{
	if(0 < y)
	{
		x = x << y;
	}
	else if(0 > y)
	{
		x = x >> (y * -1);
	}
	
	return x;
}	

int IsPow2Loop(uint num)
{
	int count = 0;
	int i = 0;
	uint cmp = 0;
	
	for(i = 0; i < INTLEN; i++)
	{
		cmp = num & 1;
		if(1 == cmp)
		{
			++count;
		}
		num = num >> 1;
	}
	
	if(1 == count)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsPow2NoLoop(uint num)
{
	if(((num - 1) & num) == 0)
	{
		return 1;
	}
	
	return 0;
}

int AddOne(uint num)
{
	int count = 0;
	
	while (1 & num)
	{
		++count;
		num = num >> 1;
	}
	
	num = num | 1;
	
	num = num << count;
	
	return num;
}

void ThreeBitsOn(uint *arr, int arr_len)
{
	int i = 0, j = 0;
	int bit_count = 0;
	int current = 0;
	
	for(i = 0; i < arr_len; ++i)
	{
		bit_count = 0;
		current = arr[i];
		for(j = 0; j < INTLEN; ++j)
		{
			if(1 & current)
			{
				++bit_count;
			}
			current = current >> 1;
		}
		
		if(3 == bit_count)
		{
			printf("Three bits: %d\n", arr[i]);
		}
	}
}

int AreSixAndTwoOn(unsigned char c)
{
	unsigned char test = 2;
	test = test & c;
	test = test << 4;
	test = test & c;
	test = test >> 5; /* return 1 if true */
	
	return test;
}

int IsTwoOrSixOn(unsigned char c)
{
	unsigned char test = 2;
	test = test & c;
	test = test << 4;
	test = test | c;
	test = test >> 5;
	
	return test & 1;
}

int SwapThreeFive(unsigned char c)
{
	unsigned char c3 = 4;
	unsigned char c5 = 16;
	
	c3 = c & c3;
	c5 = c & c5;
	
	c = (((c3 << 1) ^ (c5 >> 1)) >> 1) ^ c;
	c = (((c3 << 1) ^ (c5 >> 1)) << 1) ^ c;
	
	return c;
}

uint DivideBy16(uint n)
{
	n = n >> 4;
	n = n << 4;
	
	return n;
}


uint ByteMirrorLoop(uint num)
{
	uint mirror = 0;
	uint right_mask = 1;
	uint left_mask = 0x80000000; 
	int i = 0;
	int move = 0;
	
	for(i = 15; i >= 0; --i)
	{
		move = (i * 2) + 1;
		mirror = (right_mask & num) << move  | (left_mask & num) >> move | mirror;
		right_mask = right_mask << 1;
		left_mask = left_mask >> 1;
	}

	return mirror;
}

uint ByteMirrorNoLoop(uint num)
{
	uint mask_bytes_right = 0x00ff00ff;
	uint mask_bytes_left = ~mask_bytes_right;
	
	uint mask_half_byte_right = 0x0f0f0f0f;
	uint mask_half_byte_left =  ~mask_half_byte_right;

	uint mask_quarters_right = 0x33333333;
	uint mask_quarters_left = ~mask_quarters_right;
	
	uint mask_bits_right = 0x55555555;
	uint mask_bits_left = ~mask_bits_right;
	

	num = num << 16 | num >> 16; /* first swaps the two pairs of bytes */
	
	num = (num & mask_bytes_right) << 8 | (num & mask_bytes_left) >> 8;
	
	num = (num & mask_half_byte_right) << 4 | (num & mask_half_byte_left) >> 4;
	
	num = (num & mask_quarters_right) << 2 | (num & mask_quarters_left) >> 2;
	
	num = (num & mask_bits_right) << 1 | (num & mask_bits_left) >> 1;
	
	return num;
}

int CountBitsLoop(uint num)
{
	int mask = 1;
	int count = 0;
	int i = 0;
	for(i = 0; i < INTLEN; ++i)
	{
		if(mask & num)
		{
			++count;
		}
		
		num = num >> 1;
	}
	
	return count;	

}

int CountBitsNoLoop(uint num)
{
	int mask_first = 0xff;
	int mask_second = 0xff00;
	int mask_third = 0xff0000;
	int mask_fourth = 0xff000000;
	int count = 0;

	LUTInit();	
	count = LUT[(mask_first & num)];
	count = count + LUT[(mask_second & num) >> 8];
	count = count + LUT[(mask_third & num) >> 16];
	count = count + LUT[(mask_fourth & num) >> 24];
	
	return count;
}


void LUTInit()
{
	int mask = 1;
	int count = 0;
	int curr = 0;
	
	int i = 0, j = 0;
	for(i = 0; i < 256; ++i)
	{
		count = 0;
		curr = i;
		
		for(j = 0; j < 8; ++j)
		{
			if(mask & curr)
			{
				++count;
			}
			curr = curr >> 1;
		}
		
		LUT[i] = count;
	}
	
}

void Swap(int *num1, int *num2)
{
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num2 ^ *num1;		
}

void PrintFloat(float f)
{
	int arr[INLEN];
	
	int i = 0;
	int j = 0;
	
	int *num = NULL;
	num = (int*)&f;
	
	for(i = 0, j = INTLEN - 1; i < INTLEN; ++i, --j)
	{
		arr[j] = 1 & *num;
		*num = *num >> 1;
	}
	
	for(i = 0; i < INTLEN; ++i)
	{
		
		if(1 == i) /* just to sign the mantissa and exponent */
		{
			printf(" | ");
		}
		if(9 == i)
		{
			printf(" | ");
		}
		printf("%d ", arr[i]);
	}
	printf("\n");

}

int main()
{
	
	return 0;
}


