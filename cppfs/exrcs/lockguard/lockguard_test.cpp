#include <iostream> // std::cout
#include <thread> // std::thread
#include <mutex> // std::mutex
#include <vector> // std::vector

#include "lockguard.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }

#define CHECK(x, exp) if((x) != (exp)) { flag = 0;  }

using namespace ilrd;

void ThreadFunction(int n, int* count, std::mutex* mutex)
{
	for(int i = 0; i < n; ++i)
	{
		LockGuard<std::mutex> lock_guard(*mutex);
		++*count;
	}
}

int MutexLockTest()
{
	int flag = 1;
	
	int count = 0;
	
	std::mutex mutex;
	
	std::thread thread_a(ThreadFunction, 3000, &count, &mutex);
	std::thread thread_b(ThreadFunction, 3000, &count, &mutex);
	thread_a.join();
	thread_b.join();
	
	CHECK(6000, count);
	
	return flag;
}

void RecursiveFunction(int max, int current, std::size_t* index, 
						std::vector<int>& vec, std::recursive_mutex* mutex)
{
	LockGuard<std::recursive_mutex> lock_guard(*mutex);
	if(current >= max)
	{
		return;
	}
	vec[*index] = current;
	++*index;
	RecursiveFunction(max, current + 1, index, vec, mutex);

}

// Create a vector of ints, 2 threads running a recursive function that fills
// the vector in the indexth place with 'current' (index shared between the 
// threads), then checking that the array is ordered (0 - 1999 twice)
int RecMutexTest() 
{
	int flag = 1;
	
	std::vector<int> array(4000);
	std::recursive_mutex mutex;
	std::size_t index = 0;
	
	std::thread thread_a(RecursiveFunction, 2000, 0, &index, std::ref(array), &mutex);
	std::thread thread_b(RecursiveFunction, 2000, 0, &index, std::ref(array), &mutex);
	
	thread_a.join();
	thread_b.join();
	
	for(int i = 0; i < 2000; ++i)
	{
		CHECK(i, array[i]);
		CHECK(i, array[i + 2000]);
	}
	
	return flag;
}

void Foofi(int n, int* count, pthread_mutex_t *mutex)
{
	for(int i = 0; i < n; ++i)
	{
		LockGuard<pthread_mutex_t> lock_guard(*mutex);
		++*count;
	}
}

int PthreadMutexTest()
{
	int flag = 1;
	
	int count = 0;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
	std::thread thread_a(Foofi, 4000, &count, &mutex);
	std::thread thread_b(Foofi, 2000, &count, &mutex);
	std::thread thread_c(Foofi, 3000, &count, &mutex);
	
	thread_a.join();
	thread_b.join();
	thread_c.join();
	
	CHECK(9000, count);
	
	return flag;
}

int main()
{
	RUN_TEST(MutexLockTest);
	RUN_TEST(RecMutexTest);
	RUN_TEST(PthreadMutexTest);
	
	return 0;
}
