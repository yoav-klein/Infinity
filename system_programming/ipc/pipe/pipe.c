#define _XOPEN_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE (30)
enum pipe_value { READ, WRITE };

void Piping()
{
	int pipefd1[2];
	int pipefd2[2];
	int read_bytes = 0;
	
	char write_buffer[BUFF_SIZE];
	char read_buffer[BUFF_SIZE];
	
	if(pipe(pipefd1))
	{
		printf("error\n");
		exit(1);
	}
	
	if(pipe(pipefd2))
	{
		printf("error\n");
		exit(1);
	}
	
	if(fork())
	{
		/*close(pipefd1[READ]);
		close(pipefd2[WRITE]);
		*/
		while(1)
		{
			printf("Parent: ");
			fgets(write_buffer, sizeof(write_buffer), stdin);
			write(pipefd1[WRITE], write_buffer, strlen(write_buffer));
			sleep(1);
			
			read_bytes = read(pipefd1[READ], read_buffer, sizeof(read_buffer));
			read_buffer[read_bytes - 1] = '\0';
			printf("\nfrom parent: %s\n", read_buffer);
		}
	}
	else
	{
	/*
		close(pipefd1[WRITE]);
		close(pipefd2[READ]);
		*/
		while(1)
		{
			sleep(1);
			read_bytes = read(pipefd1[READ], read_buffer, sizeof(read_buffer));
			read_buffer[read_bytes - 1] = '\0';
			printf("\nfrom child: %s\n", read_buffer);
			
			printf("Child: ");
			fgets(write_buffer, sizeof(write_buffer), stdin);
			write(pipefd1[WRITE], write_buffer, strlen(write_buffer));
		}	
	}
	
}

void Exp()
{
	int read_bytes;
	char buffer[30];
	while(read_bytes = read(2, buffer, sizeof(buffer)))
	{
		buffer[read_bytes - 1] = '\0';
		printf("Mine: %s\n", buffer);
	}
}


int main()
{
	Piping();
	
	Exp();
	
	return 0;
	
	
}

