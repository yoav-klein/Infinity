#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#define NUM_CONSUMERS (100)
#define NUM_PRODUCERS (50)
#define TIMES (1000)
#define Q_SIZE (100)

pthread_mutex_t g_mutex;
sem_t prod_sem;
sem_t cons_sem;

int g_queue[Q_SIZE];
int g_result[TIMES];

int g_cons_count;
int g_prod_count;

int g_cons_index;
int g_prod_index;

void *Producer(void *data)
{
	while(TIMES > __sync_fetch_and_add(&g_prod_count, 1))
	{
		if(sem_wait(&prod_sem))
		{
			return data;
		}
		
		pthread_mutex_lock(&g_mutex);
		
		if(sem_post(&cons_sem))
		{
			return data;
		}
		
		g_queue[g_prod_index % Q_SIZE] = g_prod_index;
		++g_prod_index;
		
		pthread_mutex_unlock(&g_mutex);
	}
	
	return data;
}

void *Consumer(void *data)
{	
	while(TIMES > __sync_fetch_and_add(&g_cons_count, 1))
	{
		if(sem_wait(&cons_sem))
		{
			return data;
		}
		
		pthread_mutex_lock(&g_mutex);
			
		if(sem_post(&prod_sem))
		{
			return data;
		}
		
		g_result[g_cons_index] = g_queue[g_cons_index % Q_SIZE];
		++g_cons_index;
	
		pthread_mutex_unlock(&g_mutex);
	}
	
	return data;
}

int main()
{
	int status = 0;
	int i = 0;
	int flag = 1;
	
	pthread_t cons_threads[NUM_CONSUMERS];
	pthread_t prod_threads[NUM_PRODUCERS];
	
	status = sem_init(&prod_sem, 0, Q_SIZE);
	if(0 != status)
	{
		printf("failure\n");
		
		return 1;
	}
	
	status = sem_init(&cons_sem, 0, 0);
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

	for(i = 0; i < TIMES; ++i)
	{
		printf("%d\n", g_result[i]);
		if(g_result[i] != i)
		{
			flag = 0;
		}	
	}
	
	pthread_mutex_destroy(&g_mutex);
	sem_destroy(&prod_sem);
	sem_destroy(&cons_sem);
	
	printf("Flag: %d, g_cons_count: %d, g_prod_count: %d\n", flag, g_prod_count,
	g_cons_count);
	
	return 0;
}


