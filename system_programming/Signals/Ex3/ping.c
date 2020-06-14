/***************************************
*		Ex3: PingPong using signals
*		Author: Yoav Klein
*		Reviewer: Roi Kramer
*		Date: 1.8.19
**************************************/

#include <stdio.h> /* printf */
#include <signal.h> /* sigaction */
#include <unistd.h> /* pause */

volatile sig_atomic_t flag;
volatile sig_atomic_t pid;
volatile pid_t pong_pid;

sem_t sem;

void Handler(int sig, siginfo_t *siginfo, void *data)
{
	(void)sig;
	(void)data;
	
	flag = 1;
	pong_pid = siginfo->si_pid;
}

void Ping()
{
	struct sigaction sa;
	pid_t parent_pid = 0;
	
	sa.sa_sigaction = Handler;
	sa.sa_flags = SA_SIGINFO;
	
	sigaction(SIGUSR1, &sa, NULL);
	
	pause();
	
	while(1)
	{
		printf("Ping\n");
		
		kill(pong_pid, SIGUSR1);
		
		pause();
	}
}

int main()
{
	Ping();
	
	return 0;
}
