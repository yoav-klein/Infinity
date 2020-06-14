/**************************************************
*		Sorts
*		Author: Yoav Klein
*		Reviewer: Sandra bulock
*		Date: 30.6.19
*************************************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include <math.h> /* sqrt */

#include "../include/sort.h" /* BubbleSort */

#define BITS_IN_BYTE (8)
#define NIBBLE (4)

struct arguments_for_radix
{
	void *args;
	key_to_num ktn;
	size_t shift;
};

static size_t CountForRadix(const void *data, const void *args)
{
	struct arguments_for_radix arg = *(struct arguments_for_radix*)args;
	void *user_args = arg.args;
	key_to_num user_func = arg.ktn;
	size_t shift = arg.shift;
	
	return (user_func(data, user_args) >> shift) & 0xF;
}

static void Swap(void *a, void *b, size_t size)
{
	void *temp = malloc(size);
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);
	free(temp);
}

void BubbleSort(void *base, size_t num_of_members, 
				size_t element_size, cmp_f cmp)
{
	char *runner = NULL;
	size_t i = 0, j = 0;
	
	assert(NULL != base && NULL != cmp);	
		
	for(i = num_of_members - 1; i > 0; --i)
	{
		runner = base;
		
		for(j = 0; j < i; ++j)
		{
			if(0 < cmp(runner, runner + element_size))
			{
				Swap(runner, runner + element_size, element_size);
			}
			runner = runner + element_size;
		}
	}
}

void SelectionSort(void *base, size_t num_of_members, 
					size_t element_size, cmp_f cmp)
{
	size_t min = 0;
	size_t i = 0, j = 0;
	char *runner = NULL;
	char *smallest = NULL;
	
	assert(NULL != base && NULL != cmp);
	
	for(i = 0; i < num_of_members; ++i)
	{
		runner = (char*)base + i * element_size;
		smallest = runner;
		min = i;
		
		for(j = i; j < num_of_members; ++j)
		{
			if(0 > cmp(runner, smallest))
			{
				smallest = runner;
				min = j;
			}
			runner = runner + element_size;
		}
		
		if(min != i)
		{
			Swap((char*)base + i * element_size, smallest, element_size);
		}
	}
}

static void Rotate(void *what, void *where, size_t size)
{
	void *temp = malloc(size);
	
	memcpy(temp, what, size);
	memmove((void*)((char*)where + size), where, (char*)what - (char*)where);
	memcpy(where, temp, size);
	
	free(temp);
}

void InsertionSort(void *base, size_t num_of_members, 
					size_t element_size, cmp_f cmp)
{
	char *runner = base;
	char *locator = runner;
	
	for(runner = (char*)base + element_size; 
	runner < (char*)base + element_size * num_of_members; 
	runner += element_size)
	{
		if(0 < cmp(runner - element_size, runner))
		{
			locator = runner - element_size;
			while(cmp(locator - element_size, runner) > 0 && 
					locator > (char*)base)
			{
				locator -= element_size;
			}
			
			Rotate(runner, locator, element_size);
		}
	}
}

int CountingSort(void *base, size_t num_of_members, size_t element_size,
					key_to_num ktn, const void *args, size_t range)
{
	size_t current = 0;
	short *count = (short*)calloc(range, sizeof(short));
	void *output = malloc(num_of_members * element_size);
	char *runner = base;
	size_t i = 0;
	
	if(NULL == count || NULL == output)
	{
		return -1;
	}
	
	for(runner = base;
	runner < (char*)base + num_of_members * element_size; 
	runner += element_size)
	{
		current = ktn(runner, args);
		++count[current];
	}
	
	for(i = 1; i < range; ++i)
	{
		count[i] += count[i - 1];
	}
	
	for(runner = (char*)base + (num_of_members - 1) * element_size; 
	runner >= (char*)base ; runner -= element_size)
	{
		current = ktn(runner, args);
		memcpy((char*)output + (count[current] - 1) * element_size, 
		runner, element_size);
		--count[current];
	}
	
	memcpy(base, output, element_size * num_of_members);
	
	free(count);
	
	return 0;
}

int RadixSort(void *base, size_t num_of_members, size_t element_size,
					key_to_num ktn, const void *args, size_t num_of_bytes)
{
	struct arguments_for_radix arg;
	
	size_t i = 0;
	
	arg.args = (void*)args;
	arg.ktn = ktn;
	arg.shift = 0;
	
	for(i = 0; i < num_of_bytes * BITS_IN_BYTE; i += NIBBLE)
	{
		arg.shift = i;
		
		if(-1 == CountingSort(base, num_of_members, element_size, 
		CountForRadix, (void*)&arg, 16))
		{
			return -1;
		}
	}
	
	return 0;
}

void MergeSort(int *arr, int len)
{
	int *temp = malloc(sizeof(int) * len);
	
	int i = 0, j = 0;
	int middle = len / 2;
	int k = middle;
	if(len > 2)
	{
		MergeSort(arr, len / 2);
		MergeSort(arr + middle, len / 2 + len % 2);
	}
	
	for(i = 0; i < len; ++i)
	{
		if(k == len || (arr[j] < arr[k] && j < middle))
		{
			temp[i] = arr[j];
			++j;
		}
		else
		{
			temp[i] = arr[k];
			++k;
		}
	}
	
	for(i = 0; i < len; ++i)
	{
		arr[i] = temp[i];
	}
	
	free(temp);
}


static void SwapByIndex(int arr[], int a, int b)
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

static int IsBefore(int arr[], int first, int second, enum direction direction)
{
	if(direction == ASC)
	{
		return  arr[first] < arr[second];
	}
	
	if(direction == DSC)
	{
		return arr[first] > arr[second];
	}
	
	return 0;
}

static void Heapify(int arr[], int i, int n, enum direction direction)
{
	int r = i * 2 + 2;
	int l = i * 2 + 1;
	int highest = i;
	
	if(r < n && !IsBefore(arr, r, highest, direction))
	{
		highest = r;
	}
	
	if(l < n && !IsBefore(arr, l, highest, direction))
	{
		highest = l;
	}
	
	if(highest != i)
	{
		SwapByIndex(arr, highest, i);
		
		Heapify(arr, highest, n, direction);
	}
}

void HeapSort(int arr[], size_t n, enum direction direction)
{
	size_t i = 0;
	
	for(i = n / 2 - 1; i != 0; --i)
	{
		Heapify(arr, i, n, direction);
	}
	Heapify(arr, 0, n, direction);
	
	for(i = n - 1; i != 0; --i)
	{
		SwapByIndex(arr, 0, i);
		
		Heapify(arr, 0, i, direction);
	}
}

void QuickSort(int arr[], size_t n, enum direction direction)
{
	int i = 0, j = -1;
	
	int rand_num;
	
	if(n < 2)
	{
		return;
	}
	
	rand_num = rand() % n;

	SwapByIndex(arr, n - 1, rand_num);
	
	for(i = 0; i < (int)n - 1; ++i)
	{
		if(IsBefore(arr, i, n-1, direction))
		{
			++j;
			SwapByIndex(arr, j, i);
		}
	}
	++j;
	SwapByIndex(arr, n-1, j);
	
	QuickSort(arr, j, direction);
	QuickSort(arr + j + 1, n - j - 1, direction);
}

static int BinaryRec(int arr[], int key, size_t from, size_t to, size_t *index)
{
	size_t middle = (from + to) / 2;
	
	if(from > to)
	{
		return 0;
	}
	
	if(key < arr[middle])
	{
		return BinaryRec(arr, key, from, middle - 1, index);
	}
	
	if(key > arr[middle])
	{
		return BinaryRec(arr, key, middle + 1, to, index);
	}
	
	*index = middle;
	
	return 1;
}

int BinarySearch(const int arr[], int key, size_t n, size_t *index)
{
	return BinaryRec((int*)arr, key, 0, n-1, index);
}

static int JumpRec(int arr[], int key, size_t curr, size_t n, size_t *index)
{
	size_t jump = curr + sqrt(n);
	
	if(n == curr)
	{
		return 0;
	}
	
	if(jump < n && arr[jump] <= key)
	{
		return JumpRec(arr, key, jump, n, index);
	}
	
	if(arr[curr] != key)
	{
		return JumpRec(arr, key, curr + 1, n, index);
	}
	
	*index = curr;
	
	return 1;
}

int JumpSearch(const int arr[], int key, size_t n, size_t *index)
{
	return JumpRec((int*)arr, key, 0, n, index);
}



