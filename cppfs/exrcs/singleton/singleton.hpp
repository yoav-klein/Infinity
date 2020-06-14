#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

/*
  Project: Singleton.hpp
  Date: 14.11.19
  Author: Michelle Gelfand


  Singleton is an object which occurs once per program, across all threads.
  This is an expansion upon the classic static 2004 Meyers implementation,
  also implemented using the dclp 
  
  REQUIREMENTS for T:
  T is expected to supply a default private constructor
  T has to name Singleton a friend class
  T pointer will be deleted automatically upon the program's termination
  (DO NOT DELETE IT YOURSELF)

*/


#include <mutex>    // std::mutex
#include <atomic>   // std::atomic
#include <memory>   // std::unique

#include "../lockguard/lockguard.h" // LockGuard


namespace ilrd
{

template<typename T>
class Singleton
{
public:
    static T* get_instance();

    Singleton() = delete;
    ~Singleton() = delete;

    Singleton(Singleton& other) = delete; 
    Singleton& operator=(Singleton& other) = delete; 

    Singleton(Singleton&& other) = delete; 
    Singleton&& operator=(Singleton&& other) = delete; 

private:
    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
};

/* init static members */

template <typename T> 
std::atomic<T*> Singleton<T>::s_instance(nullptr);

template <typename T> 
std::mutex Singleton<T>::s_mutex;

template<typename T>
T* Singleton<T>::get_instance()
{
    static std::unique_ptr<T> destroyer;

    T* temp = s_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);

    if (temp == nullptr) 
    {
        LockGuard<std::mutex> lock(s_mutex);
        temp = s_instance.load(std::memory_order_relaxed);/* read current status of s_instance */
    
        if (temp == nullptr) 
        {
            temp = new T;
           
            destroyer.reset(temp);
            std::atomic_thread_fence(std::memory_order_release);
            s_instance.store(temp, std::memory_order_relaxed);
        }
    }
	 std::cout << "address of s_instance: " << &s_instance << std::endl;
    return temp;
}

}//ilrd


#endif //__SINGLETON_HPP__
