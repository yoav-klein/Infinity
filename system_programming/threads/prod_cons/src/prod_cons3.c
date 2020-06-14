#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create */
#include <semaphore.h> /* sem_t */

#include "queue.h"

#define NUM_CONSUMERS (30)
#define NUM_PRODUCERS (30)
#define TIMES (1000)
#define INIT_VALUE (50)

pthread_mutex_t g_mutex;
sem_t sem_item;

queue_t *g_queue;
int g_consumes[TIMES];
int g_cons_counter;
int g_prod_counter;
int g_cons_index;

int g_prod_value = INIT_VALUE;

void *Producer(void *data)
{
	while(TIMES > __sync_fetch_and_add(&g_prod_counter, 1))
	{ 
		pthread_mutex_lock(&g_mutex);
		
		QueueEnqueue(g_queue, (void*)g_prod_value);
		
		if(sem_post(&sem_item))
		{
			printf("error\n");
			
			return data;
		}
		
		++g_prod_value;
		
		pthread_mutex_unlock(&g_mutex);
	}
	
	return data;
}

void *Consumer(void *data)
{	
	int loc_cons_index = 0;
	
	while(TIMES > __sync_fetch_and_add(&g_cons_counter, 1))
	{
		if(sem_wait(&sem_item))
		{
			printf("error\n");
			
			return data;
		}
		
		pthread_mutex_lock(&g_mutex);
		
		data = QueuePeek(g_queue);
		QueueDequeue(g_queue);
		
		loc_cons_index = g_cons_index;
		++g_cons_index;
		
		pthread_mutex_unlock(&g_mutex);
		
		g_consumes[loc_cons_index] = (int)data;
	}
	
	return data;
}

int Init()
{
	int status = sem_init(&sem_item, 0, 0);
	if(0 != status)
	{
		printf("failure\n");
		
		return 1;
	}
	
	status = pthread_mutex_init(&g_mutex, NULL);
	if(0 != status)
	{
		printf("failure\n");
		
		return 1;
	}
	
	g_queue = QueueCreate();
	
	return 0;
}

int main()
{
	int status = 0;
	int i = 0;
	int flag = 1;
	
	pthread_t cons_threads[NUM_CONSUMERS];
	pthread_t prod_threads[NUM_PRODUCERS];

	if(Init())
	{
		return 1;
	}
	
	for(i = 0; i < NUM_PRODUCERS; ++i)
	{
		status = pthread_create(&prod_threads[i], NULL, Producer, NULL);
	
		if(0 != status)
		{
			printf("creation failed\n");
			
			pthread_mutex_destroy(&g_mutex);
			return 1;
		}
	}
	
	for(i = 0; i < NUM_CONSUMERS; ++i)
	{
		status = pthread_create(&cons_threads[i], NULL, Consumer, NULL);

		if(0 != status)
		{
			printf("creation failed\n");
			
			pthread_mutex_destroy(&g_mutex);
			return 1;
		}
	}
	
	for(i = 0; i < NUM_PRODUCERS; ++i)
	{
		status = pthread_join(prod_threads[i], NULL);
	
		if(0 != status)
		{
			printf("join failed\n");
			
			pthread_mutex_destroy(&g_mutex);
			return 1;
		}
	}
	
	for(i = 0; i < NUM_CONSUMERS; ++i)
	{
		status = pthread_join(cons_threads[i], NULL);
	
		if(0 != status)
		{
			printf("join failed\n");
			
			pthread_mutex_destroy(&g_mutex);
			return 1;
		}
	}
	
	pthread_mutex_destroy(&g_mutex);
	sem_destroy(&sem_item);
	
	for(i = 0; i < TIMES; ++i)
	{
		printf("%d: %d\n", i,  g_consumes[i]);
		if(g_consumes[i] != i + INIT_VALUE)
		{
			flag = 0;
		}	
	}
	
	printf("Flag: %d, if it's 1 then it's fine!\n", flag);

	return 0;
}


