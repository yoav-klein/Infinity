#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

void SimpleShell()
{
	#define MAX_ARGS 15
	#define BUFFER 50
	char **args = malloc(sizeof(*args) * MAX_ARGS);
	char *buffer = malloc(sizeof(*buffer) * BUFFER);
	char *tok = NULL;
	int i = 0;
	pid_t child_pid = 0;
	
	while (strcmp(buffer, "exit") != 0)
	{
		printf("SimpleShell$: ");
		fgets(buffer, BUFFER, stdin);
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
		wait();
	
	}
	
	free(args);
	free(buffer);
}

int main(int argc, char **argv)
{
	SimpleShell();
	
	
	return 0;
}
