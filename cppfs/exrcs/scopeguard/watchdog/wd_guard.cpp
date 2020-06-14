#include <iostream> // std::cout
#include <unistd.h> // sleep
#include "../scope_guard.h"

extern "C"
{
	#include "staying_alive.h"
}

class WatchDogWrap
{
public:
	
	WatchDogWrap(const char** argv)
		: m_argv(argv) { }

	void StartWatching() 
	{
		std::cout << "StartWatching" << std::endl; // just for logging
		wd = KeepMeAlive(m_argv);  
	}
	
	void StopWatching() 
	{ 
		std::cout << "StopWatching" << std::endl;
		LetMeDie(wd);
	}

private:
	const char** m_argv;
	watchdog_t* wd;
};

int main(int argc, const char **argv)
{
	WatchDogWrap wd(argv);
	ScopeGuard<WatchDogWrap,
				 &WatchDogWrap::StartWatching, 
				 &WatchDogWrap::StopWatching> guard(wd);
				 
	int time_to_sleep = 30;
	while(0 < time_to_sleep)
	{
		time_to_sleep = sleep(time_to_sleep);
	}
	
	return 0;
}
