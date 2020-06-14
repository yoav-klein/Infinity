#include <iostream>
#include <queue>
#include <algorithm>
#include <memory>
#include <condition_variable>
#include <mutex>

template <class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type> >
class Wrapper
{

private:
	std::priority_queue<T, Container, Compare> m_pqueue;
};

class Person
{
public:
	Person(int age)
		: m_age(age) { }
	bool operator<(const Person& other) const
	{
		return m_age < other.m_age;
	}
	
	void print() const { std::cout << m_age << std::endl; } 

private:
	int m_age;
};



void print(const Person& p)
{
	p.print();
}

template<class T>
void foo(T& obj)
{
	std::unique_ptr<int> upa(new int(3));
	obj = std::move(upa);
}

void unique_pointer()
{
	std::unique_ptr<int> upb;
	
	foo(upb);

	std::cout << *upb;
}


// i wanted to check if 'wait' can take just a boolean as a predicate,
// it can't. must be a labmda expression
void cv_wait()
{ 
	std::mutex mutex;
	std::condition_variable cv;
	bool should_stop;
	 
	std::unique_lock<std::mutex> lock(mutex);
	cv.wait(lock, [=](){ return should_stop; });
}

int main() 
{
	unique_pointer();
	/*Wrapper<int> wrapper;
	
	//std::priority_queue<int, std::vector<int>, std::less<int> > p;
	std::priority_queue<Person, std::vector<Person>, std::less<Person> > pq;
	Person folks[10] = { 20, 40, 15, 100, 92, 30, 55, 48, 73, 1 };
	
	for(int i = 0; i < 10; ++i)
	{
		pq.push(folks[i]);
	}
	
	for(int i = 0; i < 10; ++i)
	{
		pq.top().print();
		pq.pop();
	}*/
	
	return 0;
}
