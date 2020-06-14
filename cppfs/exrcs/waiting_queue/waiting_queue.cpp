#include <iostream> // cout
#include <queue>  // std::queue
#include <thread> // std::thread

#include "waiting_queue.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }

#define TIMES_PER_THREAD (100)

using namespace ilrd;

void Pusher(WaitableQueue<int, std::queue>& queue, std::size_t times, int value)
{
	for(std::size_t i = 0; i < times; ++i)
	{
		queue.Push(value);
	}
}

void Popper(WaitableQueue<int, std::queue>& queue, std::size_t times)
{
	for(std::size_t i = 0; i < times; ++i)
	{
		queue.Pop();
	}
}

void TimedPopper(WaitableQueue<int, std::queue>& queue, bool* res, 
													std::chrono::seconds secs)
{
	*res = queue.Pop(secs);
}

int TestRegularPop()
{
	
	int flag = 1;
	
	WaitableQueue<int, std::queue> queue; 
	
	std::thread thread_pop(Popper, std::ref(queue), 1);
	std::chrono::seconds seconds(1);
	std::this_thread::sleep_for(seconds);
	
	queue.Push(1);
	
	thread_pop.join();
	
	CHECK(queue.Empty(), true);
	
	return flag;
}

int TestTimedPop()
{
	int flag = 1;
	
	bool result;
	WaitableQueue<int, std::queue> queue;
	
	std::thread thread_pop(TimedPopper, std::ref(queue), &result, 
												std::chrono::seconds(3));
	
	thread_pop.join();
	
	CHECK(result, false);
	CHECK(queue.Empty(), true);
	return flag;
}

int TestTimedWaitAndWake()
{
	int flag = 1;
	
	bool result;
	WaitableQueue<int, std::queue> queue;
	
	std::thread thread_pop(TimedPopper, std::ref(queue), &result, 
													std::chrono::seconds(8));
	
	std::chrono::seconds seconds(2);
	std::this_thread::sleep_for(seconds);
	
	queue.Push(2);
	
	thread_pop.join();
	
	CHECK(result, true);
	CHECK(queue.Empty(), true);
	return flag;
}

int TestArrayOfThreads()
{
	int flag = 1;
	
	const std::size_t num_pushers = 5;
	const std::size_t num_poppers = 10;
	
	WaitableQueue<int, std::queue> queue;
	
	std::thread pushers[num_pushers];
	std::thread poppers[num_poppers];
	for(std::size_t i = 0; i < num_poppers; ++i)
	{
		poppers[i] = std::thread(Popper, std::ref(queue), 50);
	}
	
	for(std::size_t i = 0; i < num_pushers; ++i)
	{
		pushers[i] = std::thread(Pusher, std::ref(queue), 100, 40);
	}
	
	for(std::size_t i = 0; i < num_poppers; ++i)
	{
		poppers[i].join();

	}
	
	for(std::size_t i = 0; i < num_pushers; ++i)
	{
		pushers[i].join();
	}
	
	return flag;
}


int main() 
{
	RUN_TEST(TestRegularPop);
	RUN_TEST(TestTimedPop);
	RUN_TEST(TestTimedWaitAndWake);
	RUN_TEST(TestArrayOfThreads);
	
	
	return 0;
}
