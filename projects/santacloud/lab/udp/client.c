#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

#include <arpa/inet.h> 

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

void PingMaster(int sockfd)
{
	int n = 0;
	struct sockaddr_in masteraddr;
	socklen_t len = sizeof(masteraddr);
	bzero(&masteraddr, sizeof(masteraddr));
	
	masteraddr.sin_family = AF_INET;
	masteraddr.sin_port = htons(PORT);
	masteraddr.sin_addr.s_addr = INADDR_ANY;
	
	if(-1 == connect(sockfd, (struct sockaddr*)&masteraddr, len))
	{
		perror("connect");
		exit(1);
	}
	
	n = write(sockfd, "Hello", 5);
	if(-1 == n)
	{
		perror("sendto");
		exit(1);
	}
	
	
}

void ReceiveRequests(int sockfd)
{
	char *buffer = malloc(1024);
	
	char *request = malloc(50);
	
	
	while(1)
	{
		char c = 1;
		int read_bytes;
		size_t offset;
		int length;
		int req_id;
		int req_type;
		char trash;
		
		bzero(buffer, 1024);
		
		printf("reading request...\n");
		read_bytes = read(sockfd, request, 50);
		printf("read bytes: %d\n", read_bytes);
		sscanf(request, "%lu;%d;%d;%d;%c", &offset, &length, &req_id, &req_type, 
			   &trash);
		printf("offset: %lu, length: %d, req_id: %d, req_type: %d\n", offset,
			   length, req_id, req_type);
		
		if(req_type == 1)
		{
			read_bytes = read(sockfd, buffer, length);
			printf("read bytes: %d\n", read_bytes);
			printf("%s\n", buffer);
		}
	}
	free(buffer);
	free(request);
}


int main()
{
	int sockfd = CreateSocket();	
	
	PingMaster(sockfd);
	
	ReceiveRequests(sockfd);
	
	return 0;
}
