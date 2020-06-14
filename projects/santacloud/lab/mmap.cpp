#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>

void try_mmap()
{
	int fd = open("file.txt", O_RDWR);
	if(-1 == fd)
	{
		perror("couldn't open file");
		exit(1);
	}
	
	if(-1 == truncate("file.txt", 4096))
	{
		perror("truncate");
		exit(1);
	}
	
	void* addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	char* base = reinterpret_cast<char*>(addr);
	if(nullptr == base)
	{
		perror("mmap");
	}
	
	base += 30;
	char c = '0';
	
	while(c != 'q')
	{
		std::cin >> c;
		*base = c;
		++base;
	}
}

int main()
{
	try_mmap();


	return 0;
}
