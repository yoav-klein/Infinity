#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/stack.h"

#define RUN_TEST(x) if(x()) { printf(#x " OK!\n"); }\
else { printf(#x " Failed!\n"); }

struct Car
{
	char *type;
	size_t number;
	char *color;
};

int STACKTestFirst()
{
	int flag = 1;
	int res = 0;
	int test_var_i = 22;
	
	stack_t *stack = STACKCreate(3, 5);
	
	res = STACKPush(stack, &test_var_i);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = STACKPush(stack, &test_var_i);
	res = STACKPush(stack, &test_var_i);
	res = STACKPush(stack, &test_var_i);
	if(1 != res)
	{
		flag = 0;
	}
	
	res = STACKSize(stack);
	if(3 != res)
	{
		flag = 0;
	}
	
	res = *((int*)STACKPeek(stack));
	if(22 != res)
	{
		flag = 0;
	}
	
	STACKPop(stack);
	
	res = STACKSize(stack);
	if(2 != res)
	{
		flag = 0;
	}
	
	STACKPop(stack);
	STACKPop(stack);
	STACKPop(stack);
	
	test_var_i = 2;
	STACKPush(stack, &test_var_i);
	res = STACKSize(stack);
	if(1 != res)
	{
		flag = 0;
	}
	
	res = *((int*)STACKPeek(stack));
	if(2 != res)
	{
		flag = 0;
	}
	
	STACKDestroy(stack);
	
	return flag;
}

int STACKTestSecond()
{
	int flag = 1;
	int res = 0;
	struct Car *car_ptr = NULL;
	
	struct Car first_car = { "Toyota", 7392259, "White" };
	struct Car second_car = { "Mazda", 9941808, "Yellow" }; 
	
	stack_t *stack = STACKCreate(3, sizeof(first_car));
	
	res = STACKPush(stack, &first_car);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = STACKSize(stack);
	if(1 != res)
	{
		flag = 0;
	}
	
	res = STACKPush(stack, &first_car);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = STACKSize(stack);
	if(2 != res)
	{
		flag = 0;
	}
 	
 	res = STACKPush(stack, &second_car);
	if(0 != res)
	{
		flag = 0;
	}
	
	res = STACKSize(stack);
	if(3 != res)
	{
		flag = 0;
	}
	
	res = STACKPush(stack, &second_car);
	if(1 != res)
	{
		flag = 0;
	}
	
	car_ptr = (struct Car*)STACKPeek(stack);
	if(0 != strcmp(car_ptr->color, "Yellow"))
	{
		flag = 0;
	}
	
	if(0 != strcmp(car_ptr-> type, "Mazda"))
	{
		flag = 0;
	}
	
	if(9941808 != car_ptr->number)
	{
		flag = 0;
	}
	
	STACKPop(stack);
	
	car_ptr = (struct Car*)STACKPeek(stack);
	if(0 != strcmp(car_ptr->color, "White"))
	{
		flag = 0;
	}
	
	if(0 != strcmp(car_ptr-> type, "Toyota"))
	{
		flag = 0;
	}
	
	if(7392259 != car_ptr->number)
	{
		flag = 0;
	}
	STACKPop(stack);
	
	car_ptr = (struct Car*)STACKPeek(stack);
	if(0 != strcmp(car_ptr->color, "White"))
	{
		flag = 0;
	}
	
	if(0 != strcmp(car_ptr-> type, "Toyota"))
	{
		flag = 0;
	}
	
	if(7392259 != car_ptr->number)
	{
		flag = 0;
	}
	
	STACKPop(stack);
	
	
	
	STACKDestroy(stack);
	
	return flag;
}

int STACKTestThird()
{	
	void *ptr = NULL;
	int a = 9;
	stack_t *stack = STACKCreate(2, 4);
	
	(void)ptr;
	STACKPush(stack, &a);
	
	ptr = STACKPeek(stack);

	STACKPush(stack, &a);
	
	ptr = STACKPeek(stack);
	
	STACKPop(stack);
	
	ptr = STACKPeek(stack);

	STACKPop(stack);
	
	STACKDestroy(stack);
	return 1;
}

int main()
{
	RUN_TEST(STACKTestFirst);
	RUN_TEST(STACKTestSecond);
	RUN_TEST(STACKTestThird);

	
	return 1;
}
