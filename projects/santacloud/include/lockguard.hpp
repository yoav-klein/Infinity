
#include <mutex>
#include <thread>

#ifndef __LOCK_GUARD_H__
#define __LOCK_GUARD_H__

namespace ilrd
{

template<typename T>
class LockGuard
{
public:
	LockGuard(T& lock);
	~LockGuard();
	
private:
	T* m_lock;
};

template<typename T>
LockGuard<T>::LockGuard(T& lock)
	: m_lock(&lock)
{
	m_lock->lock();
}

/*
template<>
LockGuard<pthread_mutex_t>::LockGuard(pthread_mutex_t& lock)
	: m_lock(&lock)
{
	pthread_mutex_lock(m_lock);
}*/

template<typename T>
LockGuard<T>::~LockGuard()
{
	m_lock->unlock();
}

/*
template<>
LockGuard<pthread_mutex_t>::~LockGuard()
{
	pthread_mutex_unlock(m_lock);
}*/


} // namespace ilrd


#endif // __LOCK_GUARD_H__
