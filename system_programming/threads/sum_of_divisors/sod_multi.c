#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM (0xFFFFFFFFF)

struct range
{
	size_t from;
	size_t to;
	size_t *result;
};

void *SumOfDivisors(void *ranges)
{
	struct range range = *((struct range*)ranges);
	size_t sum = 0;
	size_t i = 0;
	
	for(i = range.from; i <= range.to; ++i)
	{
		if(NUM % i == 0)
		{
			sum += i;
		}
	}
	
	*(range.result) = sum;
	
	return ranges;
}

int main(int argc, char **argv)
{
	int num_threads = 0;
	int i = 0;
	size_t *results = NULL;
	pthread_t *threads = NULL;
	struct range *ranges;
	int block = 0;
	int status = 0;
	size_t total = 0;
	
	if (argc < 2)
	{
		printf("Enter num of threads\n");
		
		return 1;
	}
	
	num_threads = atoi(argv[1]);
	
	block = NUM / num_threads;

	threads = malloc(sizeof(*threads) * num_threads);
	if (NULL == threads)
	{
		printf("malloc failed\n");
		
		return 1;
	}
	
	results = malloc(sizeof(*results) * num_threads);
	if (NULL == results)
	{
		free(threads);	
		
		printf("malloc failed\n");
		
		return 1;
	}

	ranges = malloc(sizeof(*ranges) * num_threads);
	if (NULL == ranges)
	{	
		free(threads);
		free(results);
		
		printf("malloc failed\n");
		
		return 1;
	}

	for (i = 0; i < num_threads; ++i)
	{
		ranges[i].from = (i * block) + 1;
		ranges[i].to = (i * block) + block;
		ranges[i].result = results + i;
	}
	ranges[i - 1].to = NUM;

	
	for(i = 0; i < num_threads; ++i)
	{
		status = pthread_create(&threads[i], NULL, SumOfDivisors, (void*)&(ranges[i]));
		if(0 != status)
		{
			printf("couldn't create thread\n");
			
			return 1;
		}
	}
	
	for(i = 0; i < num_threads; ++i)
	{
		status = pthread_join(threads[i], NULL);
		if(status != 0)
		{
			printf("join failed\n");
			
			break;
		}	
	}
	
	for(i = 0; i < num_threads; ++i)
	{
		total += results[i];
	}
	
	printf("Total: %lu\n", total);
	
	free(threads);
	free(results);
	free(ranges);
	
	return 0;
}



