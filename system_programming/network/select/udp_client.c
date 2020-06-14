#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h> 

#define TIMES (10)
#define PORT (8080)
#define BUFF_SIZE (50)

int CreateSocket()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket creation");
		exit(1);
	}
	
	return sockfd;
}

void PingPong(int sockfd)
{
	size_t i = 0;
	int n;
	char message[BUFF_SIZE];
	char buff[BUFF_SIZE];
	struct sockaddr_in servaddr;
	socklen_t len = sizeof(servaddr);
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	for(i = 0; i < TIMES; ++i)
	{
		sprintf(message, "This is client %d", getpid());
		
		sleep(1);
		n = sendto(sockfd, message, strlen(message), 0, 
		(struct sockaddr*)&servaddr, sizeof(servaddr));
		
		if(n < 0)
		{
			perror("sendto");
			exit(1);
		}
		
		n = recvfrom(sockfd, buff, 50, 0, (struct sockaddr*)&servaddr, 
																	&len);
		if(n < 0)
		{
			perror("recvfrom");
			exit(1);
		}
		
		buff[n] = '\0';
		printf("%s\n", buff);
		
	}
	
	close(sockfd);
}

int main()
{
	int sockfd = CreateSocket();	
	
	PingPong(sockfd);
	
	return 0;
}
