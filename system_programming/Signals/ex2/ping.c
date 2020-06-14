/**********************************************
*		Signals: Exercise 2
*		Author: Yoav Klein
*		Reviewer: Roi Kramer
*		Date: 4.8.19
***********************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdio.h> /* printf */
#include <unistd.h> /* unistd */
#include <signal.h> /* sigaction */
#include <stdlib.h> /* exit */
#include <semaphore.h> /* sem_init */


sem_t sem;
volatile sig_atomic_t flag = 0;

void Handler(int sig)
{
	sem_post(&sem);
	
	(void)sig;
}

void Ping()
{
	int count = 100;
	sigset_t mask = { 0 };
	struct sigaction sa;
	pid_t child_pid = 0;
	
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	
	sa.sa_handler = Handler;
	sigaction(SIGUSR2, &sa, NULL);
	
	if(0 != sem_init(&sem, 0, 0))
	{
		perror("seminit failed");
		
		exit(1);
	}
	
	child_pid = fork();
	
	if (!child_pid)
	{
		execlp("./pong.out", "pong.out", NULL);
		
		exit(1);
	}

	kill(child_pid, SIGUSR1);
		
	while (count)
	{
		while(sem_wait(&sem))
			;
		printf("%d Parent\n", count);	
		kill(child_pid, SIGUSR1);
		--count;
	}
}

int main()
{
	Ping();
}
