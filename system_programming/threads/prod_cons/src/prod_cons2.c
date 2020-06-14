#include <stdio.h>
#include <pthread.h>
#include "queue.h"
#include <assert.h>

#define NUM_CONSUMERS (30)
#define NUM_PRODUCERS (40)
#define TIMES (500)
#define INIT_VALUE (50)

pthread_mutex_t g_mutex;

queue_t *g_queue;
int g_consumes[TIMES];
int g_cons_count;
int g_prod_value = INIT_VALUE;
volatile int g_balance;
volatile int g_total;

void *Producer(void *data)
{
	while(g_total < TIMES)
	{
		pthread_mutex_lock(&g_mutex);
		
		QueueEnqueue(g_queue, (void*)g_prod_value);
		++g_balance;
		++g_total;
		++g_prod_value;
		pthread_mutex_unlock(&g_mutex);
	}
	
	return data;
}

void *Consumer(void *data)
{	
	int loc_cons_count = 0;
	int should_write = 0;
	while(g_total < TIMES || g_balance > 0)
	{
		pthread_mutex_lock(&g_mutex);
		if(g_balance > 0)
		{	
			data = QueuePeek(g_queue);
			QueueDequeue(g_queue);
			--g_balance;
			loc_cons_count = g_cons_count;
			should_write = 1;
			++g_cons_count;
		}
		pthread_mutex_unlock(&g_mutex);
		
		if(should_write)
		{
			should_write = 0;
			g_consumes[loc_cons_count] = (int)data;
		}
	}
	
	return data;
}

int Init()
{
	int status = 0;
	
	g_queue = QueueCreate();
	
	status = pthread_mutex_init(&g_mutex, NULL);
	if(0 != status)
	{
		printf("failure\n");
		
		return 1;
	}
	
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
	
	for(i = 0; i < TIMES - 1; ++i)
	{
		printf("%d\n", g_consumes[i]);
		if(g_consumes[i] != g_consumes[i + 1] - 1)
		{
			flag = 0;
		}	
	}
	
	printf("Flag: %d\n", flag);
	
	return 0;

}
