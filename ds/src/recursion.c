#include <stdio.h>

#include "../include/recursion.h"

struct node
{
	int value;
	struct node *next;
};



long IterFibonacci(long n)
{
	int i = 0;
	int prev = 0;
	int next = 1;
	
	for(i = 0; i < n; ++i)
	{
		next = next + prev;
		prev = next - prev;
	}
	
	return prev;
}

long RecurFibonacci(long n)
{
	if(n <= 2)
	{
		return 1;
	}
	
	return RecurFibonacci(n - 1) + RecurFibonacci(n - 2);
}

size_t RecurStrlen(const char *str)
{
	if(*str != '\0')
	{
		return 1 + RecurStrlen(str + 1);  
	}
	
	return 0;
}

int RecurStrcmp(const char *str1, const char *str2)
{
	if(*str1 - *str2 == 0 && *str1 != '\0')
	{
		return RecurStrcmp(str1 + 1, str2 + 1);
	}
	
	return *str1 - *str2;
}

char *RecurStrcpy(char *dest, const char *src)
{
	*dest = *src;
	
	if('\0' != *src)
	{
		RecurStrcpy(dest + 1, src + 1);
	}
	
	return dest;
}

struct node *RecurFlip(struct node *head)
{
	struct node *ret_node = NULL;
	
	if(NULL == head->next)
	{
		return head;
	}
	
	ret_node = RecurFlip(head->next);
	head->next->next = head;
	head->next = NULL;
	return ret_node;
}

void RecurStackSort(stack_t *stack)
{
	int temp;
	int max; 
	int min;
	if(STACKSize(stack) <= 1)
	{
		return;
	}
			
	max = *(int*)STACKPeek(stack);
	STACKPop(stack);
	
	min = *(int*)STACKPeek(stack);
	STACKPop(stack);
	
	if(max < min)
	{
		temp = max;
		max = min;
		min = temp;
	} 
	
	STACKPush(stack, &max);
	
	RecurStackSort(stack);
	
	if(min > *(int*)STACKPeek(stack))
	{
		max = *(int*)STACKPeek(stack);
		STACKPush(stack, &min);
		RecurStackSort(stack);
	}
	else
	{
		STACKPush(stack, &min);
	}
}


