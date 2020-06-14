#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

#define MAP_PERM PROT_READ
#define HANDLE_ERROR(str) { perror(str); exit(EXIT_FAILURE); }
#define FILE_PERM O_RDONLY
#define NUM_THREADS (4)
#define NUM_CHARS (128)

#define ASCII_A (65)
#define ASCII_Z (90)
#define ASCII_a (97)
#define ASCII_z (122)
#define ASCII_DIFF (32)
#define WORD_SIZE (8)

int g_file_size;
int g_histogram[NUM_THREADS + 1][NUM_CHARS];

struct range
{
	char* from;
	size_t block_size;
	int *arr;
};

static void *MapFile(const char *filename);

void *SumCharsNaive(void *data)
{
	struct range range = *(struct range*)data;
	char *str = range.from;
	size_t i = 0;
	
	for(i = 0; i < range.block_size; ++i, ++str)
	{
		++range.arr[*str];
	}

	return data;
}

void *SumCharsWord(void *data)
{
	struct range range = *(struct range*)data;
	size_t *str = (size_t*)range.from;
	size_t i = 0;
	int j = 0;
	size_t words = 0;
	size_t remainder = 0;
	size_t mask = 0xFF;
	size_t current = 0;
	
	words = range.block_size / WORD_SIZE;
	remainder = range.block_size % WORD_SIZE;
	
	for(i = 0; i < words; ++i)
	{
		current = *(str + i);
		for(j = 0; j < WORD_SIZE; ++j)
		{
			++range.arr[current & mask];
			current >>= WORD_SIZE;
		}
	}
	
	current = *(str + i);
	for(i = 0; i < remainder; ++i)
	{
		++range.arr[current & mask];
		current >>= WORD_SIZE;
	}

	return data;
}

void SumHistogram()
{
	int i = 0;
	int j = 0;
	
	for(i = ASCII_a; i <= ASCII_z; ++i)
	{
		for(j = 0; j < NUM_THREADS; ++j)
		{
			g_histogram[NUM_THREADS][i] += g_histogram[j][i];
			g_histogram[NUM_THREADS][i - ASCII_DIFF] += g_histogram[j][i- ASCII_DIFF];
		}
		g_histogram[NUM_THREADS][i] += g_histogram[NUM_THREADS][i - ASCII_DIFF];
	}
}

void PrintResults()
{
	int i = 0;
	
	for(i = ASCII_a; i <= ASCII_z; ++i)
	{
		printf("%c: %9d \n", i, g_histogram[NUM_THREADS][i]);
	}
}

int main(int argc, char **argv)
{
	char *str = NULL;
	pthread_t threads[NUM_THREADS];
	struct range ranges[NUM_THREADS];
	size_t i = 0;
	size_t block_size = 0;
	
	if(argc < 2)
	{
		printf("not good!\n");
		
		return 1;
	}
	
	str = (char*)MapFile(argv[1]);
	
	block_size = g_file_size / NUM_THREADS;
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		ranges[i].from = str + i * block_size;
		ranges[i].block_size = block_size;
		ranges[i].arr = g_histogram[i];
	}
	
	ranges[i - 1].block_size = block_size + g_file_size % NUM_THREADS;
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		if(pthread_create(&threads[i], NULL, SumCharsNaive, &ranges[i]))
		{
			printf("thread create failed\n");
			
			return 1;
		}
	}
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		if(pthread_join(threads[i], NULL))
		{
			printf("join failed\n");
			
			return 1;
		}
	}
	
	SumHistogram();
	
	PrintResults();
	
	return 0;
}

static void *MapFile(const char *filename)
{
	int res = 0, fd = 0;
	struct stat file_stats;
	void *mapped_file_addr = NULL;

	fd = open(filename, FILE_PERM);

	if (-1 == fd)
	{
		HANDLE_ERROR("open");
	}

	res = fstat(fd, &file_stats);

	if (0 != res)
	{
		HANDLE_ERROR("fstat")
	}

	g_file_size = file_stats.st_size;

	mapped_file_addr = mmap(NULL, g_file_size, MAP_PERM, MAP_PRIVATE, fd, 0);

	return mapped_file_addr;

}

