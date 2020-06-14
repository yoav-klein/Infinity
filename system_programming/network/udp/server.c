#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <strings.h>

#define TIMES (20)
#define PORT (8080)

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

void Bind(int sockfd)
{
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind:");
		exit(1);
	}
}

void PingPong(int sockfd)
{
	size_t i = 0;
	size_t j = 0;
	const char *message = "hello from server!";
	char buff[1000];
	struct sockaddr_in clientaddr;
	int n = 0;
	socklen_t len = sizeof(clientaddr);
	
	bzero(&clientaddr, sizeof(clientaddr));
	
	/* change */
	for(i = 0; i < TIMES; ++i)
	{
		printf("recvfrom\n");
		n = recvfrom(sockfd, (char *)buff, 50, 
				MSG_WAITALL, ( struct sockaddr *) &clientaddr, 
				&len);  
		
		if(n < 0)
		{
			perror("recvfrom");
			exit(1);
		}
		
		buff[n] = '\0';
		printf("Server %lu: %s\n", i, buff);
		
		n = sendto(sockfd, message, strlen(message) + 1, MSG_CONFIRM, 
										(struct sockaddr *)&clientaddr, len);
		
		if(n < 0)
		{
			perror("sendto");
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
