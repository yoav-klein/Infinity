#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile atomic_t usr1;


struct sigaction 
{
	void (*handler)(int sig); 
};

void Handler(int sig)
{
	(void)sig;
	
	usr1 = 1;
}

int main()
{
	struct sigaction sa;
	
	sa.handler = Handler;
	
	sigaction(SIGUSR1, &sa, NULL);
	
	while(!usr1)
	{
		printf("PID: %d\n", getpid());
		sleep(1);
	}
	
	printf("Done\n");

	return 0;
}
