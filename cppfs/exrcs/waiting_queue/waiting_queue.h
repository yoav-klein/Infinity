
#ifndef __WAITABLE_QUEUE_H__
#define __WAITABLE_QUEUE_H__

#include <chrono> // seconds
#include <mutex> // std::mutex
#include <condition_variable> // std::condition_variable

namespace ilrd
{

template <typename T, template <class ...> class Q>	
class WaitableQueue
{
public:
	WaitableQueue() { }
	void Push(const T& data);
	void Pop();
	bool Pop(std::chrono::seconds seconds);
	T& Front();
	bool Empty();
	 
private:
	WaitableQueue(const WaitableQueue& other) { } // uncopyable
	WaitableQueue& operator=(const WaitableQueue& other) { }// uncopyable
	std::mutex m_mutex;
	std::condition_variable m_cv;
	Q<T> m_queue;
};
	
template <typename T, template <class ...> class Q>	
void WaitableQueue<T, Q>::Push(const T& data)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.push(data);
	m_cv.notify_one();
}

template <typename T, template <class ...> class Q>	
void WaitableQueue<T, Q>::Pop()
{
	while(Empty())
	{
	    std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);
	}
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.pop();
}

template <typename T, template <class ...> class Q>	
bool WaitableQueue<T, Q>::Pop(std::chrono::seconds seconds)
{
	std::cv_status status = std::cv_status::no_timeout;
	
	while(Empty() && status == std::cv_status::no_timeout)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		status = m_cv.wait_for(lock, seconds);
	}
	
	if(std::cv_status::timeout == status)
	{
		return false;
	}
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.pop();
	
	return true;
}

template <typename T, template <class ...> class Q>		
T& WaitableQueue<T, Q>::Front()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_queue.front();
}


template <typename T, template <class ...> class Q>	
bool WaitableQueue<T, Q>::Empty()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

} // namespace ilrd




#endif // __WAITABLE_QUEUE_H__
