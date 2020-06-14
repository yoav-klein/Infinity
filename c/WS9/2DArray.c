#include <stdio.h>

void TwoDArray(int (*arr)[4])
{
	printf("%d\n", arr[1][1]);
}

int main()
{
	
	int arr2[3][4] = { {1, 2, 3, 4}, {2, 4, 6, 8}, {3, 6, 9, 12} };
	int i = 0, j = 0;
	int (*parr)[4] = &arr2[0];
	int *arr = (int*)arr2;
	TwoDArray(arr);
	
	for(i = 0; i < 3; ++i)
	{
		for(j = 0; j < 4; ++j)
		{
			printf("%d\n", *(*(arr2 + i) +  j));
		}
	}
	
	
	return 1;
}
