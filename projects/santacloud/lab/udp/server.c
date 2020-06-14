

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

void WaitForSlave(int sockfd)
{
	char buffer[5];
	
	struct sockaddr_in slaveaddr;
	socklen_t len = sizeof(slaveaddr);
	int n = recvfrom(sockfd, (char*)buffer, 5, MSG_WAITALL, 
					 (struct sockaddr*)&slaveaddr, &len);
	if(-1 == n)
	{
		perror("recvfrom");
		exit(1);
	}
	
	if(!strcmp(buffer, "Hello"))
	{
		printf("Received Hello\n");
		if(-1 == connect(sockfd, (struct sockaddr*)&slaveaddr, len))
		{
			perror("connect");
			exit(1);
		}
	}
	printf("Connected to client\n");
}

void SendRequests(int sockfd)
{
	size_t offset;
	int length;
	static int req_id;
	int req_type;
	char *buffer = "This is the data of the request from master to slave";
	
	while(1)
	{
		printf("Format request:\n");
		printf("Enter offset:");
		scanf("%lu", &offset);
		printf("Enter length:");
		scanf("%d", &length);
		printf("Enter type:");
		scanf("%d", &req_type);
		
		if(1 == req_type)
		{
			length = strlen(buffer);
		}
		dprintf(sockfd, "%lu;%d;%d;%d;%c", offset, length, req_id, req_type, 0);
		if(1 == req_type)
		{
			write(sockfd, buffer, strlen(buffer));
		}
	}

}



int main()
{	
	
	int sockfd = CreateSocket();
	
	Bind(sockfd);
	
	WaitForSlave(sockfd);
	
	SendRequests(sockfd);
	
	return 0;
}
