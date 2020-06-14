/***********************************************
*		Shuffle and merge dictionary
*		Author: Yoav Klein
*		Reviewer: Mishel Liberman
*		Date: 18.8.19
*************************************************/

#include <stdio.h> /* printf */
#include <sys/stat.h> /* struct stat */
#include <fcntl.h> /* mmap */
#include <unistd.h> /* close */
#include <sys/mman.h> /* mmap */
#include <pthread.h> /* pthread_create */
#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#define DICT_FILE_PATH1 "testfile"
#define DICT_FILE_PATH "/etc/dictionaries-common/words"
#define FILE_PERM O_RDONLY
#define MAP_PERM PROT_READ | PROT_WRITE
#define NUM_THREADS 4
#define NULL_CHAR '\0'

struct map_details
{
	void *address;
	size_t size;
};

struct thread_range
{
	char **start_address;
	size_t size;
};

static struct map_details MapFile(const char *filename)
{
	int res = 0, fd = 0;
	struct stat file_stats;
	struct map_details map_details = { NULL, 0 };
	
	fd = open(filename, FILE_PERM);

	if (-1 == fd)
	{
		perror("open");
	
		return map_details;
	}

	res = fstat(fd, &file_stats);

	if (0 != res)
	{
		perror("fstat");
		
		return map_details;
	}

	map_details.size = file_stats.st_size;

	map_details.address = mmap(NULL, map_details.size + 1, MAP_PERM, MAP_PRIVATE, fd, 0);
	
	close(fd);
	
	return map_details;
}

size_t CountWords(struct map_details *map_details)
{
	size_t count_words = 0;
	char *runner = (char*)(map_details->address);
	
	*(runner + map_details->size) = NULL_CHAR;
	
	while(NULL_CHAR != *runner)
	{
		if('\n' == *runner)
		{
			++count_words;
		}
		++runner;
	}
	
	return count_words;
}

char **InitWordsArray(size_t num_of_words, char *address)
{	
	char *tok = NULL;
	char **words_array = NULL;
	int index = 0;
	
	words_array = malloc(sizeof(*words_array) * num_of_words);
	if(!words_array)
	{
		return NULL;
	}
	
	tok = strtok(address, "\n");
	
	while(NULL != tok)
	{
		words_array[index] = tok;
		++index;
		tok = strtok(NULL, "\n");
	}
	
	return words_array;
}

void SwapIndeces(char **first, char **second)
{
	char *temp;
	temp = *first;
	*first = *second;
	*second = temp;
}

void ShuffleWordArray(char **words_array, size_t size)
{
	size_t i = 0;
	int swap_index = 0;
	
	srand(time(NULL));
	
	for(i = 0; i < size; ++i)
	{
		swap_index = (rand() % (size - i)) + i;
		SwapIndeces((words_array + i), (words_array + swap_index));
	}
}

void PrintWordArray(char **address, size_t size)
{
	size_t i = 0;
	
	for(i = 0; i < size; ++i)
	{
		printf("%s\n", address[i]);
	}
	printf("Words: %lu\n", i);
}

int StrcmpWrapper(const void *first, const void *second)
{
	return strcmp(*(char**)first, *(char**)second);
}

void *Sort(void *range)
{
	struct thread_range thread_range;
	thread_range = *((struct thread_range*)range);
	
	qsort(thread_range.start_address, thread_range.size, sizeof(char*), StrcmpWrapper);
	
	return range;
}

void CopyArrays(char **dest, char **src, size_t size)
{
	size_t i = 0;
	
	for(i = 0; i < size; ++i)
	{
		dest[i] = src[i];
	}
}

int MergeSegments(struct thread_range *ranges, size_t num_of_words)
{
	char **temp_array = NULL;
	size_t i = 0;
	char **origin_array = ranges[0].start_address;
	char **min_word = NULL;
	size_t min_index = 0;
	char *max_value = "\xFF";
	size_t temp_array_ind = 0;
	
	temp_array = malloc(sizeof(*temp_array) * num_of_words);
	if(!temp_array)
	{
		perror("merge_segments:");
		
		return 1;
	}
	
	while(num_of_words > temp_array_ind)
	{
		min_index = 0;
		min_word = &max_value;
		for(i = 0; i < NUM_THREADS; ++i)
		{
			if(0 < ranges[i].size && 0 < strcmp(*min_word, *(ranges[i].start_address)))
			{
				min_word = ranges[i].start_address;
				min_index = i;
			}
		}
		
		temp_array[temp_array_ind] = *min_word;
		++temp_array_ind;
		
		ranges[min_index].start_address += 1;
		ranges[min_index].size -= 1;
	}
	
	CopyArrays(origin_array, temp_array, num_of_words);
	
	free(temp_array);
	temp_array = NULL;
	
	return 0;
}

int CreateThreadsAndMerge(char **words_array, size_t num_of_words)
{
	struct thread_range thread_structs[NUM_THREADS];
	pthread_t threads[NUM_THREADS];
	
	int i = 0;
	size_t block_size = 0;
	size_t remainder = 0;
	
	block_size = num_of_words / NUM_THREADS;
	remainder = num_of_words % NUM_THREADS;
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		thread_structs[i].start_address = words_array + (i * block_size);
		thread_structs[i].size = block_size;
	}
	thread_structs[i - 1].size += remainder;
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		if(pthread_create((threads + i), NULL, Sort, (thread_structs + i)))
		{
			perror("pthread_create:");
			
			return 1;
		}
	}
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		if(pthread_join(threads[i], NULL))
		{
			perror("pthread_join");
			
			return 1;
		}
	}
	
	return MergeSegments(thread_structs, num_of_words);
}

int main()
{
	struct map_details map_details;
	size_t num_of_words = 0;
	char **words_array = NULL;
	
	map_details = MapFile(DICT_FILE_PATH);
	if(!map_details.address)
	{
		return 1;
	}
	
	num_of_words = CountWords(&map_details);
	
	words_array = InitWordsArray(num_of_words, (char*)(map_details.address));

	PrintWordArray(words_array, num_of_words);
	
	ShuffleWordArray(words_array, num_of_words);
	
	PrintWordArray(words_array, num_of_words);
	
	CreateThreadsAndMerge(words_array, num_of_words);
	
	PrintWordArray(words_array, num_of_words);
	
	free(words_array);
	words_array = NULL;
	
	return 0;
}




