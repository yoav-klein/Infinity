#include <stdio.h> /* printf */
#include <sys/types.h>   /* socket */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* INADDR_ANY */
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <sys/select.h> /* select */
#include <unistd.h> /* read */

#define MAX_FDS (1024)
#define PORT (8080)
#define TIMES (20)
#define BUFF_SIZE (1000)

int HandleTCPReq(int fd)
{
	struct sockaddr_in cliaddr;
	int new_fd = 0;
	socklen_t addrlen = sizeof(cliaddr);
	
	memset(&cliaddr, 0, sizeof(cliaddr));
	new_fd = accept(fd, (struct sockaddr*)&cliaddr, &addrlen);
	
	return new_fd;
}


void HandleUDPReq(int fd)
{
	size_t i = 0;
	char *message = "hello from server!";
	char buff[20] = { 0 };
	struct sockaddr_in clientaddr;
	int n = 0;
	socklen_t len = sizeof(clientaddr);
	
	memset(&clientaddr, 0, sizeof(clientaddr));
	
	n = recvfrom(fd, buff, sizeof(buff), 0,
        (struct sockaddr*)&clientaddr, &len); 
		
	if(n < 0)
	{
		perror("recvfrom");
		exit(1);
	}
	
	
	buff[n] = '\0';
	printf("UDP Client: %s\n", buff);

	 if(0 > (sendto(fd, message, strlen(message), 0, 
                (struct sockaddr*)&clientaddr, sizeof(clientaddr))))
        {
            perror("failed sendto pong\n");
            exit(1);
        }
}


void HandleSTDIN()
{
	char buff[10];
	read(0, &buff, 4);
	buff[5] = '\0';
	
	if(!strcmp(buff, "Ping"))
	{
		printf("Pong\n");
	}
	if(!strcmp(buff, "Quit"))
	{
		exit(0);
	}
}

void TalkToClient(int fd)
{
	char *message = "Hello, this is server";
	char buffer[50];
	int read_bytes = 0;
	
	
	read_bytes = recv(fd, buffer, 50, 0);
	buffer[read_bytes] = '\0';

	printf("TCP client: %s\n", buffer);

	if(-1 == send(fd, message, strlen(message), 0))
	{
		perror("send");
		exit(1);
	}
	
}

void MainLoop(int tcp_sock, int udp_sock)
{
	struct timeval timeout = { 5, 0 };
	int i = 0;
	int j = 0;
	int status = 0;
	
	int fds[MAX_FDS] = { 0 };
	int curr_num_fds = 3;
	int max = tcp_sock > udp_sock ? tcp_sock : udp_sock;
	fd_set rset;
	int new_socket = 0;
	
	fds[0] = 0; /* STDIN */
	fds[1] = tcp_sock;
	fds[2] = udp_sock;
	
	while(1)
	{
		FD_ZERO(&rset);
		
		for(i = 0; i < curr_num_fds; ++i)
		{
			FD_SET(fds[i], &rset);
		}
		
		status = select(max + 1, &rset, NULL, NULL, &timeout);
		if(-1 == status)
		{
			perror("select");
			exit(1);
		}
		
		if(0 == status)
		{
			printf("timeout\n");
			exit(1);
		}
		timeout.tv_sec = 5;
		
		for(j = 0; j < curr_num_fds; ++j)
		{
			
			if(FD_ISSET(fds[j], &rset))
			{
				if(fds[j] == tcp_sock)
				{			
					new_socket = HandleTCPReq(tcp_sock);
					if(new_socket > max)
					{
						max = new_socket;
					}
					
					fds[curr_num_fds++] = new_socket;
				}
				else if(fds[j] == udp_sock)
				{
					HandleUDPReq(udp_sock);
				}
				else if(fds[j] == 0)
				{
					HandleSTDIN();
				}
				else
				{
					TalkToClient(fds[j]);
				}
			}
			
		}
	}
}

int CreateSocketTCP()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		perror("create socket tcp");
		exit(1);
	}

	
	return fd;
}


int CreateSocketUDP()
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == fd)
	{
		perror("create socket tcp");
		exit(1);
	}
	
	return fd;
}

void BindSockets(int tcp_sock, int udp_sock)
{
	struct sockaddr_in servaddr;
	socklen_t addrlen = sizeof(servaddr);
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;
	
	if(-1 == bind(tcp_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)))
	{
		perror("tcp bind");
		exit(1);
	}
	
	if(-1 == bind(udp_sock, (struct sockaddr*)&servaddr, addrlen))
	{
		perror("udp binding failed");
		exit(1);
	}
	
	if(-1 == listen(tcp_sock, 50))
	{
		perror("listen");
		exit(1);
	}
}

int main()
{
	int tcp_sock = CreateSocketTCP();
	int udp_sock = CreateSocketUDP();
	BindSockets(tcp_sock, udp_sock);
	
	MainLoop(tcp_sock, udp_sock);
	
	return 0;
}

