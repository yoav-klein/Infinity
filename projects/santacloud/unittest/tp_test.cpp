
#include <iostream>
#include <unistd.h>
#include "thread_pool.hpp"

using namespace ilrd;

class BusyWait : public TPTask
{
public:
	BusyWait();
	
	void execute() override;
};

class Sleep : public TPTask
{
public:
	Sleep(int secs);
	
	void execute();
private:
	int m_secs;
};

class Read : public TPTask
{
public:
	Read(TPTask::Priority p);
	
	void execute() override;
};

class Write : public TPTask
{
public:
	Write(TPTask::Priority p);
	
	void execute() override;
};

Read::Read(TPTask::Priority p)
	: TPTask(p)
{
}

void Read::execute()
{
	//std::cout << "Reading" << std::endl;
}

Write::Write(TPTask::Priority p)
	: TPTask(p)
{
}

void Write::execute()
{
	std::cout << "Writing" << std::endl;
}

Sleep::Sleep(int secs)
	: TPTask(TPTask::Priority::MEDIUM), m_secs(secs)
{
}

void Sleep::execute()
{
	std::cout << "Sleeping.." << std::endl;
	sleep(m_secs);
}

BusyWait::BusyWait()
	: TPTask(TPTask::Priority::HIGH) { }

void BusyWait::execute()
{
	while(1)
	{
	
	}
}

// ----------------------------------------


void suspend_resume_test()
{
	ThreadPool tp;
	
	for(int i = 0; i < 20; ++i)
	{
		std::unique_ptr<TPTask> sleep(new Sleep(2));
		tp.add_task(std::move(sleep));
		std::cout << "adding sleep task" << std::endl;
	}
	
	std::cout << "suspending" << std::endl;
	tp.suspend();
	
	sleep(4);
	
	std::cout << "resuming" << std::endl;
	tp.resume();
	
	sleep(20);
}

void stop_set_size_test()
{
	ThreadPool tp;	
	
	std::unique_ptr<TPTask> read(new Read(TPTask::Priority::HIGH));
	std::unique_ptr<TPTask> write(new Write(TPTask::Priority::LOW));
	
	tp.add_task(std::move(read));
	tp.add_task(std::move(write));
	
	char c = '0';
	while(c != 'q')
	{
		std::cout << "round again" << std::endl;
		std::cin >> c;
		std::unique_ptr<TPTask> read(new Read(TPTask::Priority::HIGH));
		tp.add_task(std::move(read));
		if(c == 's')
		{
			std::cout << "num threads: " << tp.get_size() << std::endl;
		}
		if(c == 'd')
		{
			std::cout << "enter new num" << std::endl;
			std::size_t new_size;
			std::cin >> new_size;
			tp.set_size(new_size);
		}
	}
	
	sleep(3);
	tp.stop(std::chrono::seconds(1));
	std::cout << "after stop" << std::endl;
	
	sleep(3);
}

void add_tasks(ThreadPool& tp, bool& should_go)
{
	while(should_go)
	{
		std::unique_ptr<TPTask> read(new Read(TPTask::Priority::HIGH));
		std::unique_ptr<TPTask> write(new Write(TPTask::Priority::LOW));
	
		tp.add_task(std::move(read));
		tp.add_task(std::move(write));
		sleep(1);
	}
}

void interactive_test()
{
	ThreadPool tp;
	bool should_go = true;
	std::thread tasks_adder(add_tasks, std::ref(tp), std::ref(should_go));
	
	char c = '0';
	while(c != 'q')
	{
		std::cout << "round again" << std::endl;
		std::cin >> c;
		if(c == 'n')
		{
			std::cout << "num threads: " << tp.get_size() << std::endl;
		}
		if(c == 'd')
		{
			std::cout << "enter new num" << std::endl;
			std::size_t new_size;
			std::cin >> new_size;
			tp.set_size(new_size);
		}
		if(c == 's')
		{
			tp.suspend();
		}
		if(c == 'r')
		{
			tp.resume();
		}
	}
	should_go = false;
	
	tasks_adder.join();
}

void violent_stop_test()
{
	ThreadPool tp;
	
	//std::unique_ptr<TPTask> busy_wait(new BusyWait());
	//tp.add_task(std::move(busy_wait));
	
	std::unique_ptr<TPTask> long_sleep(new Sleep(30));
	tp.add_task(std::move(long_sleep));
	
	std::cout << "violent stop" << std::endl;
	sleep(3);
	tp.stop(std::chrono::seconds(1));
}


int main()
{
	interactive_test();
	
	
	return 0;
}
