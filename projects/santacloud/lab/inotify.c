#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int count = 0;


void inotify()
{
	int fd = inotify_init();
	int wd = 0;
	char buffer[1000];
	char *ptr;
	int read_bytes = 0;
	
	if(-1 == fd)
	{
		perror("inotify_init");
		exit(1);
	}
	
	wd = inotify_add_watch(fd, "watched", IN_CREATE | IN_DELETE | IN_CLOSE_WRITE);
	if(-1 == wd)
	{
		perror("add watch");
		exit(1);
	}
	
	while(1)
	{
		printf("-----------%d-----------\n", count++);
		read_bytes = read(fd, buffer, 1000);
		printf("read bytes: %d\n", read_bytes);
		for(ptr = buffer; ptr < buffer + read_bytes; 
			ptr += sizeof(struct inotify_event) + ((struct inotify_event*)ptr)->len)
		{
			struct inotify_event* event = (struct inotify_event*)ptr;
			if(event->mask & IN_CREATE)
			{
				printf("created\n");
			}
			if(event->mask & IN_DELETE)
			{
				printf("deleted\n");
			}
			if(event->mask & IN_CLOSE_WRITE)
			{
				printf("modified\n");
			}
			printf("len: %d\n", event->len);
			printf("name: %s\n", event->name);
		}
	}
	
	printf("%lu\n", sizeof(struct inotify_event));
	close(fd);

}

int main()
{
	inotify();
	
	return 0;
}
