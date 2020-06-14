#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_key_t key;

void *FirstThread(void *args)
{
	void *value = (void*)20;
	void *ret_val = NULL;
	
	pthread_setspecific(key, value); 
	ret_val = pthread_getspecific(key);
	
	return ret_val;
}

void *SecondThread(void *args)
{
	void *ret_val = NULL;
	void *value = (void*)30;
	
	pthread_setspecific(key, value); 
	
	ret_val = pthread_getspecific(key);
	
	return ret_val;
}

int main()
{
	int key_val = pthread_key_create(&key, NULL);
	void *first_thread_val = malloc(sizeof(int));
	void *second_thread_val = malloc(sizeof(int));	
	
	pthread_t first_thread_id;
	pthread_t second_thread_id;
	
	pthread_create(&first_thread_id, NULL, FirstThread, NULL);
	pthread_create(&second_thread_id, NULL, SecondThread, NULL);
	
	pthread_join(first_thread_id, first_thread_val);
	pthread_join(second_thread_id, second_thread_val);
	
	printf("First thread: %d. Second thread: %d.\n", 
	*(int*)first_thread_val, *(int*)second_thread_val);
	
	return 0;
}
