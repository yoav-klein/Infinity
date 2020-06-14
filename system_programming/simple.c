/********************************************
*		Simple WatchDog and Simple Shell
*		Author: Yoav Klein
*		Reviewer: Yael Lotan
*		Date: 31.7.19
*********************************************/

#include <stdio.h> /* stdin */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */
#include <signal.h>
#include <unistd.h> 

void Handler(int sig)
{
	(void)sig;
	
	printf("SIGINT\n");
}

struct sigaction
{
	void (*handler)(int sig);
	int sa_flags;
};

int SimpleWatchDogSystem()
{
	int ret_val = 0;
	
	char *str = "./child.out";
	
	(void)ret_val;
	while (1)
	{
		ret_val = system(str);
	}
}

int SimpleWatchDogFork()
{
	pid_t pid_child = 0;
	char **argv = NULL;
	argv = malloc(sizeof(char*));
	if (NULL == argv)
	{
		return 1;
	}
	
	argv[0] = "child.out";
	
	while (1)
	{
		pid_child = fork();
		if(pid_child == 0)
		{
			execv("./child.out", argv);
		}
		wait(NULL);
	}
	
	free(argv);
	return 0;
}


void SimpleShell()
{
	struct sigaction sig;
		
	#define MAX_ARGS 15
	#define BUFFER 50
	char **args = NULL; 
	char *tok = NULL;
	int i = 0;
	pid_t child_pid = 0;
	

	char *buffer = malloc(sizeof(*buffer) * BUFFER);
	if(NULL == buffer)
	{
		free(args);
		
		return;
	}
		
	args = malloc(sizeof(*args) * MAX_ARGS);
	if(NULL == args)
	{
		return;
	}
	
	sig.handler = Handler;
	sig.sa_flags = 0;
	sigaction(SIGINT, &sig, NULL);
	
	while (strcmp(buffer, "exit") != 0)
	{
		printf("SimpleShell$: ");
		fgets(buffer, BUFFER, stdin);
		
		if (0 == strcmp(buffer, "exit"))
		{
			break;
		}
		
		tok = strtok(buffer, " \n");

		args[0] = tok;
		i = 1;
		
		while (NULL != tok && strcmp(buffer, "exit") != 0)
		{
			tok = strtok(NULL, " \n");
			args[i] = tok;
			++i;
		}
		
		child_pid = fork();
		if (child_pid == 0)
		{
			execvp(args[0], args);
			printf("%s: unknown command\n", args[0]);
			exit(-1);
		}
		wait(NULL);
	
	}
	
	free(args);
	free(buffer);
}


int main()
{
	SimpleShell();
}

