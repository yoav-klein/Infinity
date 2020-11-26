#include <stdio.h> /* printf */
#include <sys/types.h>   /* socket */
#include <sys/socket.h> /* socket */
#include <stdlib.h> /* exit */
#include <arpa/inet.h>  /* INADDR_ANY */
#include <string.h> /* memset */

#define TIMES (20)
#define PORT (8080)
#define LISTEN_BACKLOG (50)

int CreateSocket()
{
	int opt = 1; 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket creation");
		exit(1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	return sockfd;
}

void Bind(int sockfd)
{
	struct sockaddr_in servaddr;
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	if(-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
	{
		perror("bind");
		exit(1);
	}
	printf("Bound to %s\n", inet_ntoa(servaddr.sin_addr));
}

void PingPong(int sockfd)
{
	size_t i = 0;
	int cfd;
	socklen_t client_addr_size;
	struct sockaddr_in cliaddr;
	
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	if(-1 == listen(sockfd, LISTEN_BACKLOG))
	{
		perror("listen");
		exit(1);
	}
	
	printf("After listen\n");
	client_addr_size = sizeof(cliaddr);
	cfd = accept(sockfd, (struct sockaddr*)&cliaddr, &client_addr_size);
	if(-1 == cfd)
	{
		perror("accept");
		exit(1);
	}
	printf("Received connection from: %s\n", inet_ntoa(cliaddr.sin_addr));
	printf("New socket: %d\n", cfd);
	for(i = 0; i < TIMES; ++i)
	{
		char buffer[50];
		ssize_t n = 0;
		char message[20];
		
		n = recv(cfd, buffer, 50, 0);
		if(-1 == n)
		{
			perror("recv");
			exit(1);
		}
		buffer[n] = '\0';
		
		printf("Server: %s\n", buffer);
		sprintf(message, "Hello from server no. %lu", i);
		if(-1 == send(cfd, message, strlen(message), 0))
		{
			perror("send");
			exit(1);
		}
	}
}

int main()
{
	int sockfd = CreateSocket();
	
	Bind(sockfd);
	
	PingPong(sockfd);
	
	
	
	return 0;
}
