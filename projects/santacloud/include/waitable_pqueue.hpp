//wrapper over priority queue
//with waitable pop, and with the addition of timeout 
//
//Author: 


#ifndef _WAITABLE_PQUEUE_H_
#define _WAITABLE_PQUEUE_H_

#include <chrono> // std::chrono::seconds
#include <mutex> // std::mutex
#include <condition_variable> // std::condition_variable
#include <vector> // std::vector
#include <queue> // std::priority_queue
#include <utility> // std::forward

namespace ilrd
{

template <class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type> >
class WaitablePQueue
{
public:
    WaitablePQueue() = default;
    ~WaitablePQueue() = default;

    void push(const T& data);
    void push(T&& data);
    void pop(T& return_obj);
    bool pop(T& return_obj, std::chrono::seconds seconds);
    bool empty() const;
    size_t size() const;

    WaitablePQueue(const WaitablePQueue& other) = delete;
    WaitablePQueue(WaitablePQueue&& other) = delete;
    WaitablePQueue& operator=(const WaitablePQueue& other) = delete;
    WaitablePQueue& operator=(WaitablePQueue&& other) = delete;

private:
	std::priority_queue<T, Container, Compare> m_pqueue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
};


template <class T, class Container, class Compare>
void WaitablePQueue<T, Container, Compare>::push(const T& data)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_pqueue.push(std::move(data));
	m_cv.notify_one();
}
    
template <class T, class Container, class Compare>
void WaitablePQueue<T, Container, Compare>::push(T&& data)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_pqueue.push(std::move(data));
	m_cv.notify_one();
}  

template <class T, class Container, class Compare>
void WaitablePQueue<T, Container, Compare>::pop(T& return_obj)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [=](){return !m_pqueue.empty();});
	
	return_obj = std::move(const_cast<T&>(m_pqueue.top()));
	m_pqueue.pop();
}

template <class T, class Container, class Compare>
bool WaitablePQueue<T, Container, Compare>::pop(T& return_obj, 
												std::chrono::seconds seconds)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	
	bool not_timeout; 
	not_timeout = m_cv.wait_for(lock, seconds, [=](){return !m_pqueue.empty();});
	if(!not_timeout)
	{
		return false;
	}
	
	return_obj = std::move(const_cast<T&>(m_pqueue.top()));
	m_pqueue.pop();
	
	return true;
}
    
template <class T, class Container, class Compare>
bool WaitablePQueue<T, Container, Compare>::empty() const
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_pqueue.empty();
}

template <class T, class Container, class Compare>
size_t WaitablePQueue<T, Container, Compare>::size() const
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_pqueue.size();
}



}   //namespace ilrd

#endif //_WAITABLE_PQUEUE_H_
