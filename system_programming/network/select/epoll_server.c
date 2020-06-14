#include <stdio.h> /* printf */
#include <sys/types.h>   /* socket */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* INADDR_ANY */
#include <stdlib.h> /* exit */
#include <string.h> /* memset */

#include <unistd.h> /* read */
#include <sys/epoll.h> /* epoll_create */

#define MAX_FDS (1024)
#define PORT (8080)
#define TIMES (20)
#define BUFF_SIZE (1000)


/*******************************
	The problem with this file is that when a 
	TCP client quits, (closes it's socket), the 
	epoll_wait will receive requests from it.
	each time you receive a request from a TCP client
	you need to somehow check if it's still on.
**********************************/

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
	read(0, &buff, 10);
	buff[4] = '\0';
	
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

	printf("TCP client fd: %d, read_bytes: %d, %s\n", fd, read_bytes, buffer);
	if(read_bytes > 0)
	{
		if(-1 == send(fd, message, strlen(message), 0))
		{
			write(1, "send", 4);
			perror("send");
			exit(3);
		}
	}
	printf("tcp_after_send\n");
}

void MainLoop(int tcp_sock, int udp_sock)
{
	int j = 0;
	int nfds = 0;
	
	struct epoll_event current_event;
	struct epoll_event events[5];
	int epfd = epoll_create(10);
	if(-1 == epfd)
	{
		perror("epoll_create");
		exit(1);
	}
	
	/* add tcp_sock */
	current_event.data.fd = tcp_sock;
	current_event.events = EPOLLIN;
	if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, current_event.data.fd, 
						&current_event))
	{
		perror("epoll_ctl");
		exit(1);
	}
	
	/* add udp_sock */
	current_event.data.fd = udp_sock;
	current_event.events = EPOLLIN;
	if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, current_event.data.fd, 
						&current_event))
	{
		perror("epoll_ctl");
		exit(1);
	}
	
	/* add stdin */
	current_event.data.fd = 0;
	current_event.events = EPOLLIN;
	if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, current_event.data.fd, 
						&current_event))
	{
		perror("epoll_ctl");
		exit(1);
	}
	
	while(1)
	{
		static int count = 0;
		printf("round again\n");
		nfds = epoll_wait(epfd, events, 5, 100000);
		printf("count: %d\n", count++);
		if(-1 == nfds)
		{
			perror("epoll_wait");
			exit(1);
		}
		
		if(0 == nfds)
		{
			printf("timeout\n");
			exit(1);
		}
		
		for(j = 0; j < nfds; ++j)
		{
			printf("fd: %d\n", events[j].data.fd);
			if(events[j].data.fd == tcp_sock)
			{			
				int new_socket = HandleTCPReq(tcp_sock);
				current_event.data.fd = new_socket;
				current_event.events = EPOLLIN;
				if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, current_event.data.fd, 
						&current_event))
				{
					perror("epoll_ctl");
					exit(1);
				}
				
			}
			else if(events[j].data.fd == udp_sock)
			{
				HandleUDPReq(udp_sock);
			}
			else if(events[j].data.fd == 0)
			{
				HandleSTDIN();
			}
			else
			{
				TalkToClient(events[j].data.fd);
			}
			
		}
	}
	printf("exit\n");
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
	printf("start\n");
	BindSockets(tcp_sock, udp_sock);
	
	MainLoop(tcp_sock, udp_sock);
	printf("exit\n");
	return 0;
}

