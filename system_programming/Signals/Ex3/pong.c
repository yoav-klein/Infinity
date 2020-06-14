/***************************************
*		Ex3: PingPong using signals
*		Author: Yoav Klein
*		Reviewer: Roi Kramer
*		Date: 1.8.19
**************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* atoi */
#include <signal.h>	 /* sigaction */
#include <sys/types.h> /* pid_t */

volatile sig_atomic_t flag;

void Handler(int sig)
{
	(void)sig;
	
	flag = 1;
}

void Pong(char *ping_pid)
{
	struct sigaction sa;
	pid_t parent_pid = 0;
	
	ping_pid = atoi(ping_pid);
	sa.sa_handler = Handler;
	
	sigaction(SIGUSR1, &sa, NULL);	
	flag = 1;
	
	while (1)
	{
		if (flag)
		{
			flag = 0;
			printf("Pong\n");
			kill(ping_pid, SIGUSR1);
		}
	}
	
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Enter Ping's pid\n");
		
		exit(1);
	}
	
	Pong(argv[1]);
	
	return 0;	
}


