#include <stdio.h>


int mult(int prev, int *curr, int *res, int count)
{
	if(0 == count)
	{
		return 1;
	}
	
	int next = mult(*curr * prev, curr + 1, res + 1, --count);
	*res = prev * next;
	
	return next * (*curr);
}

void print(int *arr, int count)
{
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		printf("%d ", arr[i]);
	}
	
	printf("\n");
}

int main()
{
	int arr[4] = { 1, 2, 3, 4 };
	int res[4];
	
	mult(1, arr, res, 4);
	
	print(res, 4);
	
	
	return 0;
}
