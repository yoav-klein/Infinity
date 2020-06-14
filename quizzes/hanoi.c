#include <stdio.h>
#include "../include/stack.h"

void Hanoi(stack_t *from, stack_t *to, stack_t *holder, size_t n)
{
	int temp = 0;
	
	if (1 == n)
	{
		temp = *(int*)STACKPeek(from);
		STACKPop(from);
		STACKPush(to, &temp);
	}
	else
	{
		Hanoi(from, holder, to, n - 1);
		temp = *(int*)STACKPeek(from);
		STACKPop(from);
		STACKPush(to, &temp);
		Hanoi(holder, to, from, n - 1);
	}
}

int main()
{
	int n = 30;
	size_t i = 0;
	int arr[100];
	
	stack_t *stack1 = STACKCreate(n, 4);
	stack_t *stack2 = STACKCreate(n, 4);
	stack_t *stack3 = STACKCreate(n, 4);
	
	for(i = 0; i < n; ++i)
	{
		arr[i] = i + 1;
		STACKPush(stack1, &arr[i]);
	}
	
	Hanoi(stack1, stack2, stack3, n);
	
	for(i = 0; i < n; ++i)
	{
		printf("%d\n", *(int*)STACKPeek(stack2));
		STACKPop(stack2);
	}
	
	STACKDestroy(stack1);
	STACKDestroy(stack2);
	STACKDestroy(stack3);
}
	
