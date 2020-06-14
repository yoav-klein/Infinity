// checking what happens if you call close(-1),
// to see if i can create a move ctor to FileDescriptor
// in which i'll replace the other's fd to -1, and it will 
// call close on it.

// another alternative is trying to resize the vector and then 
// use the opertaor=

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "file_raii.hpp"

using namespace ilrd;

void operator_equal()
{
	std::vector<FileDescriptor> fds;
	fds.reserve(4);
}	

int main()
{
	operator_equal();
	
	return 0;
}
