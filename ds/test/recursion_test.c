#include <stdio.h>

#include "../include/recursion.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

struct node
{
	int value;
	struct node *next;
};


int FibboIter_Test()
{
	int flag = 1;
	
	if(13 != IterFibonacci(7))
	{
		flag = 0;
	}
	
	return flag;
}

int FibboRec_Test()
{
	int flag = 1;
	if(13 != RecurFibonacci(7))
	{
		flag = 0;
	}
	
	if(1 != RecurFibonacci(2))
	{
		flag = 0;
	}
	
	return flag;
}

int Strlen_Test()
{
	int flag = 1;
	size_t count1 = 0;
	size_t count2 = 0;
	char *str1 = "Mexico";
	char *str2 = "";
	
	count1 = RecurStrlen(str1);
	if(6 != count1)
	{
		flag = 0;
	}
	
	count2 = RecurStrlen(str2);
	if(0 != count2)
	{
		flag = 0;
	}
	
	return flag;
}

int Strcmp_Test()
{
	int flag = 1;
	int result = 0;
	char *str1 = "Yoav Klein";
	char *str2 = "Yoav Klein";
	char *str3 = "Yoav Tlekn";
	
	result = RecurStrcmp(str1, str2);
	if(0 != result)
	{
		flag = 0;
	}
	
	result = RecurStrcmp(str1, str3);
	if(0 == result)
	{
		flag = 0;
	}
	
	return flag;
}

int Strcpy_Test()
{
	int flag = 1;
	int result;
	char *src = "Eli Ohana";
	char dest[10];
	char *ret_val;
	
	ret_val = RecurStrcpy(dest, src);
	result = RecurStrcmp(src, dest);
	if(0 != result || dest != ret_val)
	{
		flag = 0;
	}
	
	return flag;
}

int FlipTest()
{
	int flag = 1;
	
	struct node *new_head;
	
	struct node first;
	struct node second;
	struct node third;
	struct node fourth;
	
	first.value = 4;
	first.next = &second;
	
	second.value = 10;
	second.next = &third;
	
	third.value = 20;
	third.next = &fourth;
	
	fourth.value = 1;
	fourth.next = NULL;
		
	new_head = RecurFlip(&first);
	
	if(fourth.next != &third)
	{
		flag = 0;
	}
	
	if(third.next != &second)
	{
		flag = 0;
	}
	
	if(second.next != &first)
	{
		flag = 0;
	}
	
	if(first.next != NULL)
	{
		flag = 0;
	}
	
	if(new_head != &fourth)
	{
		flag = 0;
	}
	
	return flag;
}

int StackSort_Test()
{
	int i = 0;
	int flag = 1;
	stack_t *stack = STACKCreate(10, 4);
	int arr[10] = {300 ,200 ,100 , 80, 50, 40, 30, 20, 10, 5 };
	int temp = 0;
	
	for(i = 0; i < 10; ++i)
	{
		STACKPush(stack, &arr[i]);
	}
	
	for(i = 0; i < 9; ++i)
	{
		temp = *(int*)STACKPeek(stack);
		STACKPop(stack);
		if(temp > *(int*)STACKPeek(stack))
		{
			flag = 0;
		}
	}
	
	STACKDestroy(stack);
	
	return flag;
}

int main()
{
	RUN_TEST(FibboIter_Test);
	RUN_TEST(FibboRec_Test);
	RUN_TEST(Strlen_Test);
	RUN_TEST(Strcmp_Test);
	RUN_TEST(Strcpy_Test);
	RUN_TEST(FlipTest);
	RUN_TEST(StackSort_Test);
	
	
	return 0;
}



