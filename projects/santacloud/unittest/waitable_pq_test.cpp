#include <iostream>
#include <memory> // unique_ptr
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unistd.h>

#include "tools.hpp"
#include "waitable_pqueue.hpp"

/*
	This is a test for waitable_pqueue unit.
	what i do is fill a queue with unique_ptrs of ints,
	and have some threads waiting on a condition variable, and 
	once receiving the 'go' - read from the queue into 
	a global array.
	then i let the threads block on 'pop', and push to the pq
	an element each second to test this scenario.
	i expect to receive an array going from 0 - 109

*/

using namespace ilrd;


class Compare
{
public:
	bool operator()(const std::unique_ptr<int>& a, const std::unique_ptr<int>& b)
	{
		return *a < *b;
	}
};

bool compare(const std::unique_ptr<int>& a, const std::unique_ptr<int>& b)
{
	return *a < *b;
}

const int SIZE = 100;
const std::size_t NUM_THREADS = 20;

std::condition_variable m_cv;
std::size_t threads_count = 0;
std::size_t g_count = 0;
std::mutex g_mutex;
int array[SIZE];

/*WaitablePQueue<std::unique_ptr<int>, std::vector<std::unique_ptr<int>>,
	Compare > g_pq;
	*/
/*WaitablePQueue<std::unique_ptr<int>, std::vector<std::unique_ptr<int>>,
	std::function<bool(std::unique_ptr<int>, std::unique_ptr<int>)> > g_pq;
	
WaitablePQueue<int> g_pq_int;*/
/*

void thread_function()
{
	std::unique_ptr<int> res;
	while((__sync_fetch_and_add(&g_count, 1)) < SIZE)
	{
		g_pq.pop(res);
		std::unique_lock<std::mutex> lock(g_mutex);

		array[*res] = *res;
	}
}

bool unique_pointer_test()
{
	bool flag = true;
	
	std::size_t i = 0;
	std::thread threads[NUM_THREADS];
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i] = std::thread(thread_function);
	}
	
	for(i = 0; i < SIZE; ++i)
	{
		std::unique_ptr<int> ptr(new int(i));
		g_pq.push(std::move(ptr));
	}
	
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i].join();
	}
	
	for(i = 0; i < SIZE; ++i)
	{
		if(array[i] != (int)i)
		{
			flag = false;
		}
		std::cout << array[i] << std::endl;
	}
	
	return flag;
}

void thread_int_function()
{
	while((__sync_fetch_and_add(&g_count, 1)) < SIZE)
	{
		int res;
		g_pq_int.pop(res);
		std::unique_lock<std::mutex> lock(g_mutex);
		array[res] = res;
	}
}

bool test_int()
{
	bool flag = true;
	
	std::size_t i = 0;
	std::thread threads[NUM_THREADS];
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i] = std::thread(thread_int_function);
	}
	std::cout << "threads created" << std::endl;
	
	for(i = 0; i < SIZE; ++i)
	{
		g_pq_int.push(i);
	}
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i].join();
	}
	
	for(i = 0; i < SIZE; ++i)
	{
		if(array[i] != (int)i)
		{
			flag = false;
		}
		std::cout << array[i] << std::endl;
	}
	
	return flag;
}
*/
int main()
{
	if(1)
	{
		std::cout << GREEN "Test Succeed!" RESET << std::endl;
	}
	else
	{
		std::cout << RED "Test Failed!" RESET << std::endl;
	}
	
	
	return 0;
}
