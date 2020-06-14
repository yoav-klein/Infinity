/*****************************************
*		Producers Consumers
*		Author: Yoav Klein
*		Reviewer: Eldad Carin
*		Date: 8.8.19
*******************************************/

#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create */
#include <unistd.h> /* sleep */

#define ARR_SIZE 1000

volatile int g_flag = 1;
int sec_flag = 1;

int current = 1;
int counter = 0;
int counter_write = 0;

int results[ARR_SIZE];
int arr[ARR_SIZE];

int SumArray(int *arr)
{
	int i = 0;
	int sum = 0;
	
	for(i = 0; i < ARR_SIZE; ++i)
	{
		sum += arr[i];
	}
	
	return sum;
}

void *Reading(void *data)
{
	int temp_arr[ARR_SIZE];
	int i = 0;
	
	while (1)
	{
		int count_temp = 0;
		
		while(0 == (__sync_val_compare_and_swap(&g_flag, 1, 0)))
			;
		
		if(sec_flag == 0)
		{
			for (i = 0; i < ARR_SIZE; ++i)
			{
				temp_arr[i] = arr[i];
			}
			count_temp = counter - 1;
		
			printf("counter: %d, sum: %d\n", count_temp, SumArray(temp_arr));
	
			sec_flag = 1;
		}
		
		g_flag = 1;
	}
	
	return data;
}

void *Writing(void *data)
{
	int i = 0;
	
	while (1)
	{
		while(0 == (__sync_val_compare_and_swap(&g_flag, 1, 0)))
			;
		
		if(1 == sec_flag)
		{
			for (i = 0; i < ARR_SIZE; ++i)
			{
				arr[i] = counter;
			}
			++counter;
			sec_flag = 0;
		}
		
		g_flag = 1;
	}
	
	return data;
}

int main()
{
	pthread_t write_thread;
	pthread_t read_thread;
	pthread_attr_t attr;
	int status = 0;
	
	status = pthread_attr_init(&attr);
	if (status != 0)
	{
		printf("init failed\n");
		
		return 1;
	}
	
	status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (0 != status)
	{
		printf("setdetach failed\n");
		
		return 1;
	}
	
	status = pthread_create(&write_thread, &attr, Writing, NULL);
	if (0 != status)
	{
		printf("write creation failed\n");
		
		return 1;
	}
	
	status = pthread_create(&read_thread, &attr, Reading, NULL);
	if (0 != status)
	{
		printf("read creation failed\n");
		
		return 1;
	}
	
	sleep(10);
	
	return 0;
}
