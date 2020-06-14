#include <iostream>
#include <chrono>
#include <thread>
#include <queue>


void milli()
{
	std::chrono::milliseconds mill;
	//std::chrono::time_point<std::chrono::high_resolution_clock> clock;
	while(true)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1, 1000> > diff = end - begin;
		
		std::cout << diff.count() << std::endl;
		
		std::cout << (begin > end) << std::endl;
		
		std::cout << "sleep" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000) - diff);
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1, 1000> > diff2 = now - begin;
		std::cout << "wake" << diff2.count() << std::endl;
	}
}


struct TimeStamp
{
	std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
};

bool operator<(const TimeStamp& lhs, const TimeStamp& rhs)
{
	return lhs.m_time > rhs.m_time;
}

void map()
{
	std::priority_queue<TimeStamp> queue;
	TimeStamp early;
	early.m_time = std::chrono::high_resolution_clock::now();
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	TimeStamp late;
	late.m_time = std::chrono::high_resolution_clock::now();
	
	
	queue.push(early);
	queue.push(late);
	
	std::chrono::time_point<std::chrono::high_resolution_clock> top = queue.top().m_time;
	queue.pop();
	std::chrono::time_point<std::chrono::high_resolution_clock> buttom = queue.top().m_time;
	
	std::chrono::duration<double, std::ratio<1, 1000> > diff = top - buttom;
	
	std::cout << diff.count() << std::endl;
}

int main()
{
	map();
	
	//milli();
	return 0;
}
