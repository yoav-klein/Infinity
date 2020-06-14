/******************************************************************
*						Struct of function pointers
*						Author: Yoav Klein
*						Reviewer: Alex Zilberman
*						Date: 23.5.19
*****************************************************************/


#include <string.h> /* strcpy */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "data.h" 

#define UNUSED(x) (void)(x)
#define SIZEOFSTR 20

typedef struct data_holder
{
	void *data;
	void (*add_fp)(void**, int num);
	void (*print_fp)(void*);
	void (*free_fp)(void*);
} data_t;

data_t *GetArray() /* Creates a static array and returning it's address */
{

	static data_t data_arr[3];
	
	char *str = (char*)malloc(sizeof(char) * SIZEOFSTR); 
	if(NULL == str)
	{
		printf("Allocation failed\n");
		exit(0);
	}
	strcpy(str, "Jerusalem");
	
	data_arr[0].data = (void*)10;
	data_arr[0].add_fp = AddToInt;
	data_arr[0].print_fp = PrintInt;
	data_arr[0].free_fp = DoNothing;
	
	*(float*)&data_arr[1].data = 3.14;
	data_arr[1].add_fp = AddToFloat;
	data_arr[1].print_fp = PrintFloat;
	data_arr[1].free_fp = DoNothing;
	
	data_arr[2].data = (void*)str;
	data_arr[2].add_fp = AddToString;
	data_arr[2].print_fp = PrintString;
	data_arr[2].free_fp = FreeString;
	
	return data_arr;
}


void AddToInt(void **data_ptr, int num)
{
	*((int*)data_ptr) += num;  	
}

void AddToFloat(void **data_ptr, int num)
{
	*((float*)data_ptr) += num;
}

void AddToString(void **data_ptr, int num)
{
	char *str = (char*)*data_ptr;
	sprintf(str, "%s%d", str, num);
}

void PrintInt(void *data)
{
	printf("%ld\n", (long)data);
}

void PrintFloat(void *data)
{
	printf("%f\n", *(float*)&data);
}

void PrintString(void *data)
{
	char *str = (char*)data;
	printf("%s\n", str);
}

void FreeString(void *ptr)
{
	free(ptr);
}

void DoNothing(void *ptr)
{
	UNUSED(ptr);
}

int main()
{
	data_t *data_arr;
	
	int i = 0;
	
	data_arr = GetArray();
	for(i = 0; i < 3; ++i)
	{
		data_arr[i].add_fp(&(data_arr[i].data), 5);
		data_arr[i].print_fp(data_arr[i].data);
		data_arr[i].free_fp(data_arr[i].data);
	}
	
	return 1;
}

