/**
 * Thread Pool follows Object Pool Design Pattern.
 * Threads perform the tasks according to their priority. 
 * 
 * A Task must provide:
 *      1) execute() function 
 *      2) operator < (for std::less)
 */


#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <thread>               // std::thread
#include <unordered_map>        // std::unordered_map
#include <chrono>               //std::chrono::seconds
#include <memory> 				//std::unique_ptr
#include <condition_variable>   // std::condition_variable
#include <mutex> 				// std::mutex

#include "logger.hpp" 			// Logger
#include "tptask.hpp"			// TPTask
#include "waitable_pqueue.hpp"  // WaitablePQueue

namespace ilrd
{

class ThreadPool;

struct StopException
{
};


// execute of TaskStop decreases m_thread_to_collect of the ThreadPool,
// signs the flag of the executing thread to flase, and throws a StopException
// to be caught by the exec_tasks() function.
class TaskStop : public TPTask
{
public:
	TaskStop(ThreadPool& tp);
	void execute();
	
private:
	ThreadPool& m_tp;
};


// execute of TaskSuspend waits on the condition variable until m_is_suspended
// evaluates to false
class TaskSuspend : public TPTask
{
public:
	TaskSuspend(ThreadPool& tp);	
	void execute();
	
private:
	ThreadPool& m_tp;
};

class ThreadPool
{

public:
    static const size_t DEFAULT_NUM_OF_THREADS = 4;
    
    //if hardware_concurrency not supported please enter value manually
    explicit ThreadPool(size_t thread_num = std::thread::hardware_concurrency());

    ~ThreadPool();

    // Adding Task to the queue (when task finished its exex. it's removed from 
    // the queue)
    void add_task(std::unique_ptr<TPTask> task);

    // Setting new ammount of threads (incrementing / decrementing). 
    // If the new ammount is less than the prev. thus the <delta> threads that 
    // finish their tasks first will be removed from the pool.
    void set_size(size_t size);

    // Returns the total number of threads in the pool
    size_t get_size() const;
	
    // Permenantly stops after timeout, 
    // (!) caution:resources may be lost.
    void stop(std::chrono::seconds timeout);

    // Temporarily pauses the threads immediately after finnishing of the 
    // currently running tasks, without danger of losing resources.
    void suspend();

    // Resumes the suspended threads.
    void resume();

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

private:
	friend class TaskStop;
	friend class TaskSuspend;
	
	void exec_tasks();
	void add_stop_tasks();
	void collect_threads(bool with_timeout, std::chrono::seconds timeout);
	void create_threads(std::size_t amount);
	void add_suspend_tasks(std::size_t amount);
	void resume_threads();
	void join_finished_threads(std::vector<std::thread::id>& threads);
	
	bool m_is_suspended;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	
    WaitablePQueue<std::unique_ptr<TPTask> > m_queue;
    std::unordered_map<std::thread::id, std::pair<std::thread, bool> > m_pool;
	
	std::size_t m_threads_to_collect;

  	Logger* m_logger;
  	void LOG(Logger::Severity sever, const std::string& msg, int line);

};

} // ilrd

#endif //__THREAD_POOL_HPP__

