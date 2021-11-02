
#include <stdio.h> /* printf */
#include <sys/epoll.h> /* epoll_create1 */
#include <unistd.h> /* read */
#include <stdlib.h> /* exit */

#define MAX_EVENTS (10)
#define BUFFSIZE (100)

void read_stdin()
{
	char buff[BUFFSIZE];
	int res = 0;
	
	res = read(0, buff, BUFFSIZE);
	buff[res] = 0;
	printf("%s", buff);
}


int main()
{
	struct epoll_event stdin_event;
	struct epoll_event events[MAX_EVENTS];
	int epoll_fd = 0;
	int i = 0;
	int nfds = 0;
	
	epoll_fd = epoll_create1(0); /* 0 is the flags parameter */
	if(-1 == epoll_fd)
	{
		perror("epoll_create1");
		exit(1);	
	}
	
	stdin_event.events = EPOLLIN;
	stdin_event.data.fd = 0;
	if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &stdin_event))
	{
		perror("epoll_ctl");
	}
	
	while(1)
	{
		printf("epoll wait\n");
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if(-1 == nfds)
		{
			perror("epoll_wait");
			exit(1);
		}
		
		for(i = 0; i < nfds; ++i)
		{
			if(0 == events[i].data.fd)
			{
				read_stdin();
			}
		}	
		
	}
	
	return 0;
}
