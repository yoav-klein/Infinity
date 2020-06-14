#include <iostream>
#include <thread>
#include <signal.h> // pthread_sigmask
#include <unistd.h> // sleep

/****************************************************
	Learn how to set a member function
	as thread function, 
	and how to mask signals for thread
	but not of process
	
	the class task simulates the NBDDriverProxy, 
	which opens a thread, the thread masks all signals,
	the main thread sets sighandler for SIGINT and SIGTERM
	that calls nbddriverproxy->disconnect.
*******************************************************/

class Task
{
public:
	Task(int a) : 
		m_i(a)
	{
		for(int i = 0; i < 3; ++i)
		{
			sleep(1);
			std::cout << "Some operations" << std::endl;
		}
		thread = std::thread(&Task::thread_function, this, 20);
	}

	~Task()
	{
		thread.join();
	}
	
	void thread_function(int a)
	{
		sigset_t sigset;
		if(-1 == sigfillset(&sigset))
		{
			perror("sigfillset"); // exception
		}
		if(-1 == pthread_sigmask(SIG_SETMASK, &sigset, NULL))
		{
			perror("pthread_sigmask"); // exception
		}
		while(m_i == 1)
		{
			sleep(1);
			std::cout << "still here " << m_i << " " << a << std::endl;
		}
	}
	
	void stop()
	{
		m_i = 0;
	}
private:
	int m_i;
	std::thread thread;
	
};

Task* task;

void sighandler(int sig)
{
	(void)sig;
	std::cout << "Here" << std::endl;
	task->stop();
}

void set_sigaction(int signal, struct sigaction* act)
{
	if(-1 == sigaction(signal, act, NULL))
	{
		perror("sigaction"); // exception ?
		exit(1);
	}
}


void set_sighandler()
{
	struct sigaction act;
	act.sa_handler = sighandler;
	if (sigemptyset(&act.sa_mask) != 0 ||
    sigaddset(&act.sa_mask, SIGINT) != 0 ||
    sigaddset(&act.sa_mask, SIGTERM) != 0) 
    {
    	perror("sigemptyset"); // exception ?
    	exit(1); 
    }
    
    set_sigaction(SIGINT, &act);
    set_sigaction(SIGTERM, &act);
}

int main()
{	
	
	set_sighandler();
	
	Task t(1);
	
	task = &t;
	
	
	
	return 0;
}
