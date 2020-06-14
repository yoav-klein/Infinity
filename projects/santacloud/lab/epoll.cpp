#include <sys/epoll.h>
#include <iostream>

// just to see the values of the macros of the different events

int main()
{
	std::cout << EPOLLRDHUP << std::endl;
	std::cout << EPOLLPRI << std::endl;
	std::cout << EPOLLHUP << std::endl;
	std::cout << EPOLLET << std::endl;
	std::cout << EPOLLIN << std::endl;
	std::cout << EPOLLOUT << std::endl;
	std::cout << EPOLLWAKEUP << std::endl;
	std::cout << EPOLLERR << std::endl;
	std::cout << EPOLLONESHOT << std::endl;
	
	
	return 0;
}
