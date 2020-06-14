/***********************************************
*		Signals: Exercise 1
*		Author: Yoav Klein
*		Reviewer: Roi Kramer
*		Date: 4.8.19
************************************************/

#include <stdio.h> /* write */
#include <signal.h> /* sigaction */
#include <unistd.h> /* getppid */
#include <sys/types.h> /* pid_t */

#define STDOUT (1)

volatile sig_atomic_t flag = 0;

void Handler(int sig)
{
	flag = 1;	
	(void)sig;
}

void PingPong()
{
	size_t counter = 0;
	struct sigaction sa = {0};
	pid_t child_pid = 0;

	sa.sa_handler = Handler;
	
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	
	child_pid = fork();
	
	if(child_pid)
	{
		kill(child_pid, SIGUSR1);
	}
	
	while(counter < 500)
	{
		if(child_pid && flag)
		{
			flag = 0;
			write(STDOUT, "P\n", 2);
			kill(child_pid, SIGUSR1);	
			++counter;
		}
		if(0 == child_pid && flag)
		{
			flag = 0;
			write(STDOUT, "C\n", 2);
			kill(getppid(), SIGUSR2);
			++counter;
		}
	}
}

int main()
{
	PingPong();
	
	return 0;
}
