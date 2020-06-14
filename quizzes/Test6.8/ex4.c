#include <stdio.h>

#define SIZE (10)

void Swap(char *first, char *second)
{
	char temp = *first;
	*first = *second;
	*second = temp;
}

void ShiftInPlace(char *arr, int n, int shift)
{
	int count = 0;
	int i = 0;
	
	if(0 == shift)
	{
		return;
	}
	
	for (i = shift; i < n; ++i)
	{
		Swap(&arr[i], &arr[count % shift]);
		++count;
	}
	
	ShiftInPlace(arr, shift, (shift - (n % shift)) % shift);
}

void PrintArray(char *arr)
{
	int i = 0;
	
	for (i = 0; i < SIZE; ++i)
	{
		printf("%c ", arr[i]);
	}
	
	printf("\n");
	
}

int main()
{
	char arr[SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
	
	ShiftInPlace(arr, SIZE, 8);
	
	PrintArray(arr);
	
	return 0;
}
