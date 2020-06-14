
#include "lockguard.hpp"

template<>
LockGuard<pthread_mutex_t>::LockGuard(pthread_mutex_t& lock)
	: m_lock(&lock)
{
	pthread_mutex_lock(m_lock);
}


template<>
LockGuard<pthread_mutex_t>::~LockGuard()
{
	pthread_mutex_unlock(m_lock);
}

