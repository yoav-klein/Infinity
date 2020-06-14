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
#include <semaphore.h> /* sem_init */
#include <stdlib.h> /* exit */

volatile sig_atomic_t flag = 0;
sem_t sem;

void Handler(int sig)
{
	(void)sig;

	sem_post(&sem);
}

void Pong()
{
	struct sigaction sa = { 0 };
	sigset_t mask = { 0 };
	pid_t parent_pid = getppid();
	
	sa.sa_handler = Handler;
	sigaction(SIGUSR1, &sa, NULL);
	
	if(sem_init(&sem, 0, 0))
	{
		perror("sem_init failed");
		
		exit(1);
	}
	
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
	
	while(parent_pid == getppid())
	{
		while(sem_wait(&sem))
			;
		printf("Child\n");
		flag = 0;
		kill(parent_pid, SIGUSR2);
	}
}

int main()
{
	Pong();
}
