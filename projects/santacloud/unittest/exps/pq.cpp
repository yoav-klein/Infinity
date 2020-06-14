#include <iostream>
#include <queue>
#include <functional>
#include <memory>
#include <stdlib.h> 
#include <time.h> 

class Dog
{
public:
	Dog(int a)
		: m_a(a) { }
	
	bool operator<(const Dog& other)
	{
		return m_a < other.m_a;
	}


	int m_a;
};

class Predicate
{
public:
	bool operator()(int a, int b)
	{
		return a < b;
	}

};

bool predicate(int a, int b)
{
	return a < b;
}

<<<<<<< HEAD
bool operator<(const std::unique_ptr<Dog>& dog1, 
			   const std::unique_ptr<Dog>& dog2)
=======
bool operator<(const std::unique_ptr<Dog>& dog1, const std::unique_ptr<Dog>& dog2)
>>>>>>> 743e215298d4afc973a4afba3847270149d278b6
{
	//std::cout << "here" << std::endl;
	return *dog1 < *dog2;
}

void dogs()
{
	srand(time(NULL));
	
	std::priority_queue<std::unique_ptr<Dog> > pq;
	
	for(int i = 0; i < 20; ++i)
	{
		int num = rand() % 100;
		std::unique_ptr<Dog> ptr(new Dog(num));
		pq.push(std::move(ptr));
	}
	
	while(!pq.empty())
	{
		Dog* ptr = pq.top().get();
		std::cout << ptr->m_a << std::endl;
		pq.pop();
	}
	
}

int main()
{
/*
	std::function<bool(int, int)> pred;
	std::priority_queue<int, std::vector<int>, 
								  std::function<bool(int, int)>> pq(predicate);
	
	pq.push(2);
	pq.push(3);
	pq.push(1);
	pq.push(33);
	pq.push(22);
	
	while(!pq.empty())
	{
		std::cout << pq.top() << std::endl;
		pq.pop();
		
	}*/
	
	dogs();
	

	return 0;
}
