#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *Action(void *args)
{
	size_t count_j = 0;
	size_t count_i = 0;
	size_t i = 0;
	size_t j = 0;
	(void)args;
	
	for(i = 0; i < 1000; ++i)
	{
		++count_i;
		for(j = 0; j < 5000; ++j)
		{
			++count_j;		
		}
		
		pthread_testcancel();
	}
	
	pthread_exit((void*)count_i);
	
	return (void*)count_i;
}

int main()
{
	size_t count = 0;
	void *ret_val = malloc(sizeof(void*));
	
	pthread_t thread_id;
	
	pthread_create(&thread_id, NULL, Action, ret_val);
	
	while(1)
	{
		++count;
		printf("Another main\n");
		
		if(1000 == count)
		{
			pthread_cancel(thread_id);
		}
		
		if(100000 == count)
		{
			printf("Sending Cancellantion request\n");
			
			break;
		}
	}
	
	pthread_join(thread_id, ret_val);
	
	printf("main: %lu, thread: %lx\n", count, *(size_t*)ret_val);
	printf("PTHREAD_CANCELLED: %lx\n", PTHREAD_CANCELED);
	
	return 0;
}
