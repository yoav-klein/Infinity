#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE (80)

int main()
{
	char buff[BUFF_SIZE];
	char in_buff[BUFF_SIZE];
	int fd = 0;
	size_t bytes_read = 0;
	mkfifo("fifo.dat", 0666);
	
	while(1)
	{
		printf("Enter a string:\n");
		fgets(buff, sizeof(buff), stdin);
		fd = open("fifo.dat", O_RDWR);
		write(fd, buff, strlen(buff));
		close(fd);
		
		sleep(1);
		fd = open("fifo.dat", O_RDWR);
		bytes_read = read(fd, in_buff, BUFF_SIZE);
		close(fd);
		
		in_buff[bytes_read - 1] = '\0';
		printf("%s\n", in_buff);
		
	}
	
	
	return 0;
}
