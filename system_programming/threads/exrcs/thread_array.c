#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ARR_SIZE 100000

int arr[ARR_SIZE];

void *WriteToArray(void *index)
{
	arr[(size_t)index] = (size_t)index + 1;
}

int main()
{
	pthread_t *thread_ids = NULL;
	pthread_attr_t attr;
	size_t i = 0;
	size_t j = 0;
	int status = 0;
	unsigned int sleep_val = 0;
	size_t index = 0;
	
	thread_ids = malloc(sizeof(*thread_ids) * ARR_SIZE);
	if(NULL == thread_ids)
	{
		printf("malloc failed\n");
		
		return 1;
	}
	
	status = pthread_attr_init(&attr);
	
	if (0 != status)
	{
		printf("attr_init failed\n");
		
		return 1;
	}
	
	for (i = 0; i < ARR_SIZE; ++i)
	{
		status = pthread_create(&thread_ids[i], &attr, WriteToArray, (void*)i);
		
		if(0 != status)
		{
			for (j = index; j < i; ++j)
			{
				status = pthread_join(thread_ids[j], NULL);
				if (0 != status)
				{
					printf("join failed\n");
					
					return 1;
				}
			}
			--i;
			
			index = i + 1;	
		}
	}
	
	for(j = index; j < ARR_SIZE; ++j)
	{
		status = pthread_join(thread_ids[j], NULL);
			
		if (0 != status)
		{
			printf("join failed\n");
					
			return 1;
		}
			
	}
	
	for(j = 0; j < i ; ++j)
	{
		if(arr[j] != j + 1)
		{
			printf("Not Good\n");
			
			break;
		}
	}
	
	status = pthread_attr_destroy(&attr);
	if(0 != status)
	{
		printf("pthread_attr_destroy failed\n");
	}
	printf("Good\n");
	
	
	
	return 0;

}
