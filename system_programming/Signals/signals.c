
/*******************************************************8
*		signals.h
*
*	This file is a demonstration of using sighandler in linux programming
*
*
*
*************************************************************/


#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void handler(int signum)
{
	printf("received a %d\n", signum);
}

int main()
{
	struct sigaction sa = { 0 };
	sa.sa_handler = handler;
	if(-1 == sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		exit(1);
	}
	
	while(1)
	{
		printf("Zzz..\n");
		sleep(10);
	}
	
}
