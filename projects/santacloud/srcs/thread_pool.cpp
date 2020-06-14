#include <algorithm> // std::for_each
#include <iostream>
#include "thread_pool.hpp"
#

namespace ilrd
{

bool operator<(const std::unique_ptr<TPTask>& lhs, 
			   const std::unique_ptr<TPTask>& rhs)
{
	return *lhs < *rhs;
}

TaskStop::TaskStop(ThreadPool& tp)
	: TPTask(TPTask::Priority::STOP), m_tp(tp) { }
	
void TaskStop::execute()
{
	std::lock_guard<std::mutex> lock(m_tp.m_mutex);
	--m_tp.m_threads_to_collect;
	m_tp.m_pool[std::this_thread::get_id()].second = false; 
	m_tp.m_cv.notify_all();
	
	throw StopException();
}

TaskSuspend::TaskSuspend(ThreadPool& tp)
	: TPTask(TPTask::Priority::SUSPEND), m_tp(tp) { }
	
void TaskSuspend::execute()
{
	std::unique_lock<std::mutex> lock(m_tp.m_mutex);
	m_tp.m_cv.wait(lock, [this](){ return !m_tp.m_is_suspended; } );
}


/*******************************************/


Logger* get_logger();


ThreadPool::ThreadPool(size_t thread_num)
	: m_is_suspended(false),  m_threads_to_collect(0), m_logger(get_logger())

{
	thread_num = thread_num > 0 ? thread_num : DEFAULT_NUM_OF_THREADS;
	create_threads(thread_num);
}

void ThreadPool::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, "ThreadPool", msg, line);
}

ThreadPool::~ThreadPool()
{
	LOG(Logger::DEBUG, "dtor", __LINE__);

	stop(std::chrono::seconds(4));
}	


void ThreadPool::add_task(std::unique_ptr<TPTask> task)
{
	m_queue.push(std::move(task));
}


void ThreadPool::stop(std::chrono::seconds timeout)
{
	m_threads_to_collect = m_pool.size();
	
	add_stop_tasks();
	resume_threads(); // in case suspended

	collect_threads(true, timeout);

	LOG(Logger::DEBUG, "all threads joined", __LINE__);

}

size_t ThreadPool::get_size() const
{
	return m_pool.size(); 
}

void ThreadPool::set_size(size_t size)
{
	int delta = (int)size - (int)m_pool.size();
	
	if(0 <= delta)
	{
		if(m_is_suspended)
		{
			add_suspend_tasks(delta);
		}
		create_threads(delta);	
	}
	else
	{
		add_stop_tasks();
		if(!m_is_suspended) // if suspended, will be collected only on resume
		{
			collect_threads(false, std::chrono::seconds(0));
		}
	}
}

void ThreadPool::suspend()
{
	m_is_suspended = true;
	
	add_suspend_tasks(m_pool.size());
}

void ThreadPool::resume()
{
	resume_threads();
	if(m_threads_to_collect) // size decreased while suspended
	{
		collect_threads(false, std::chrono::seconds(0));
	}
}


//------------- private functions ------------------


void ThreadPool::resume_threads()
{
	m_is_suspended = false;
	m_cv.notify_all();
}

void ThreadPool::collect_threads(bool with_timeout, std::chrono::seconds timeout)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	std::vector<std::thread::id> threads_to_delete;
	
	if(with_timeout)
	{
		if(!m_cv.wait_for(lock, timeout, [this](){ return !m_threads_to_collect;}))
		{	
			for(auto& iter : m_pool)
			{
				pthread_cancel(iter.second.first.native_handle());
				iter.second.first.join();
				threads_to_delete.push_back(iter.first);
			}
		}
		else
		{
			join_finished_threads(threads_to_delete);
		}
	}
	else
	{
		m_cv.wait(lock, [=](){return !m_threads_to_collect;});
		join_finished_threads(threads_to_delete);
	}
	
	for(std::size_t i = 0; i < threads_to_delete.size(); ++i)
	{
		m_pool.erase(threads_to_delete[i]);
	}
}

void ThreadPool::join_finished_threads(std::vector<std::thread::id>& threads)
{
	for(auto& iter : m_pool)
	{
		if(false == iter.second.second)
		{
			iter.second.first.join();
			threads.push_back(iter.first);
		}
	}
}

void ThreadPool::exec_tasks()
{
	try
	{
		while(true)
		{
			std::unique_ptr<TPTask> task;
			m_queue.pop(task);
			task->execute();
		}
	}
	catch(StopException& e)
	{
	}
}

void ThreadPool::create_threads(std::size_t amount)
{
	for(std::size_t i = 0; i < amount; ++i)
	{
		std::thread thread(&ThreadPool::exec_tasks, this);
		
		std::thread::id id = thread.get_id();
		std::pair<std::thread, bool> pair(std::move(thread), true);
		m_pool.emplace(id, std::move(pair));
	}
}

void ThreadPool::add_suspend_tasks(std::size_t amount)
{
	for(std::size_t i = 0; i < amount; ++i)
	{
		add_task(std::unique_ptr<TPTask>(new TaskSuspend(*this)));
	}
}


void ThreadPool::add_stop_tasks()
{
	std::size_t num_to_collect = m_threads_to_collect; 
	for(std::size_t i = 0; i < num_to_collect; ++i)
	{
		add_task(std::move(std::unique_ptr<TPTask>(new TaskStop(*this))));
	}
}

} // ilrd
