#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char buffer[20];
	int fd = open("./BUSE-master/folder/file.txt", O_RDWR);
	ssize_t n = 0;
	
	if(-1 == fd)
	{
		perror("open file");
		exit(1);
	}
	
	n = read(fd, buffer, 20);
	buffer[n] = '\0';
	
	printf("%s\n", buffer);
	

	return 0;
}
