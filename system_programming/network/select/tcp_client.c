#include <stdio.h> /* printf */
#include <sys/types.h>   /* socket */
#include <sys/socket.h> /* socket */
#include <stdlib.h> /* exit */
#include <arpa/inet.h>  /* INADDR_ANY */
#include <string.h> /* memset */
#include <unistd.h> /* getpid */

#define TIMES (10)
#define PORT (8080)
#define LISTEN_BACKLOG (50)
#define BUFF_SIZE (50)

int CreateSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		perror("create socket tcp");
		exit(1);
	}
	
	return fd;
}

void Connect(int sockfd)
{
	struct sockaddr_in servaddr;
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;
	
	if(-1 == connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
	{
		perror("connect");
		exit(1);
	}
}

void PingPong(int sockfd)
{
	int status = 0;
	char message[BUFF_SIZE];
	char buffer[BUFF_SIZE];
	int i = 0;
	
	for(i = 0; i < TIMES; ++i)
	{
		sprintf(message, "Hello server, this is %d, %d", getpid(), i);
		
		sleep(1);
		status = send(sockfd, message, strlen(message), 0);
		if(-1 == status)
		{
			perror("send");
			exit(1);
		}
	
		status = recv(sockfd, buffer, BUFF_SIZE, 0);
		if(-1 == status)
		{
			perror("recv");
			exit(1);
		}
	
		buffer[status] = '\0';
	
		printf("%s\n", buffer);
	}
}

int main()
{
	int fd = CreateSocket();
	Connect(fd);
	PingPong(fd);
	
	close(fd);
	
	return 0;
}
