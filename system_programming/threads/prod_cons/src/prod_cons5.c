#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#define TIMES (400)
#define ARR_SIZE (1000)
#define NUM_CONSUMERS (45)

int g_arr[ARR_SIZE];
int g_result[TIMES];
pthread_mutex_t g_mutex;
sem_t g_sem;
pthread_cond_t g_cond;

int g_cons_index;
int g_res_index;

int g_value;

void *Producer(void *data)
{
	int i = 0;
	
	while(1)
	{
		
		for(i = 0; i < NUM_CONSUMERS; ++i)
		{
			sem_wait(&g_sem);
		}
		
		for(i = 0; i < ARR_SIZE; ++i)
		{
			g_arr[i] = g_value;
		}
		
		++g_value;
		
		pthread_mutex_lock(&g_mutex);
		
		pthread_cond_broadcast(&g_cond);
		
		pthread_mutex_unlock(&g_mutex);
		
		if(100 == g_value)
		{
			return data;
		}
	}
	return data;
}

void *Consumer(void *data)
{
	int sum = 0;
	int i = 0;
	
	while(1)
	{
		sum = 0;
		
		pthread_mutex_lock(&g_mutex);
		
		sem_post(&g_sem);
		
		pthread_cond_wait(&g_cond, &g_mutex);
		
		pthread_mutex_unlock(&g_mutex);
			
		for(i = 0; i < ARR_SIZE; ++i)
		{
			sum += g_arr[i];
		}
		
		pthread_mutex_lock(&g_mutex);
		
		g_result[sum / ARR_SIZE] += 1;
		
		pthread_mutex_unlock(&g_mutex);
	
		if(100 == g_value)
		{
			return data;
		}

	}
	
	return data;
}

int main()
{
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS];
	int i = 0;
	
	if(pthread_mutex_init(&g_mutex, NULL))
	{
		printf("error\n");
		
		return 1;
	}
	
	if(sem_init(&g_sem, 0, 0))
	{
		printf("error\n");
		
		return 1;
	}
	
	if(pthread_cond_init(&g_cond, NULL))
	{
		printf("error\n");
		
		return 1;
	}
	
	if(pthread_create(&producer, NULL, Producer, NULL))
	{
		printf("error\n");
		
		return 1;
	}
	
	for(i = 0; i < NUM_CONSUMERS; ++i)
	{
		if(pthread_create(&consumers[i], NULL, Consumer, NULL))
		{
			printf("error\n");
			
			return 1;
		}
	}
	
	if(pthread_join(producer, NULL))
	{
		printf("error\n");
		
		return 1;
	}
	
	for(i = 0; i < NUM_CONSUMERS; ++i)
	{
		if(pthread_join(consumers[i], NULL))
		{
			printf("error\n");
			
			return 1;
		}
	}	
	
	for(i = 0; i < 100; ++i)
	{
		printf("%d %d\n", i, g_result[i]);
	}
	
	pthread_cond_destroy(&g_cond);
	sem_destroy(&g_sem);
	pthread_mutex_destroy(&g_mutex);
	
	return 0;
	
}
