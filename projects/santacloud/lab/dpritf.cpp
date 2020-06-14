
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("file.txt", O_RDWR | O_CREAT);
	
	//dprintf(fd, "%d,%li,%s\n", 3, 12345678901234, "Testing");	
	dprintf(fd, "%d%d%c", 3, 2, 0);
	
	std::string str;
	close(fd);
	fd = open("file.txt", O_RDONLY);
	
	char c;
	read(fd, &c, 1);
	while(c != '\n')
	{
		str += c;
		
		read(fd, &c, 1);
	}
	
	std::cout << str;
	
	return 0;
}
