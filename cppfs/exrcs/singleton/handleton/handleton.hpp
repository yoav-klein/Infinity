#ifndef __HANDLETON_HPP__
#define __HANDLETON_HPP__

/*
  Project: handleton.hpp
  Date: 17.11.19
  
  Handleton is a version of singleton, only that it 
  supports a dynamic loading of shared libraries.
  the trick is, to instantiate the desired singleton instances
  i.e.: std::atomic<T*> s_instance's, in a different .cpp file, and 
  make it a shared object, and then to link the application file with this
  shared object, do the same for the explicitly loaded shared object, and then
  you have the symbol shared for the app and for the explicitly loaded shared object.
  
*/

#include <iostream> // std::cout 

#include <mutex>    // std::mutex
#include <atomic>   // std::atomic
#include <memory>   // std::unique

#include "../../lockguard/lockguard.hpp" // LockGuard

#define InitHandleton(type)\
template<>\
std::atomic<type*> Handleton<type>::s_instance(nullptr);\
template<>\
std::mutex Handleton<type>::s_mutex{};

namespace ilrd
{

template<typename T>
class Handleton
{
public:
    static T* get_instance();

    Handleton() = delete;
    ~Handleton() = delete;

    Handleton(Handleton& other) = delete; 
    Handleton& operator=(Handleton& other) = delete; 

    Handleton(Handleton&& other) = delete; 
    Handleton&& operator=(Handleton&& other) = delete; 

private:
    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
};


template<typename T>
T* Handleton<T>::get_instance()
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

    return temp;
}

template <typename T>
std::atomic<T*> Handleton<T>::s_instance;

template <typename T>
std::mutex Handleton<T>::s_mutex;

}//ilrd


#endif //__HANDLETON_HPP__
