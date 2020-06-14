#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <algorithm>

#include "disk_storage.hpp"
#include "driver_data.hpp"

#include "tools.hpp"

#define CHECK(x, y) if(x != y) { flag = false; }
#define RUN_TEST(x) if(x() == true) { std::cout << GREEN "Test OK!" RESET << std::endl; }\
					else { std::cout << RED "Test Failed!" RESET << std::endl;} 
using namespace ilrd;


// This file tests the function of DiskStorgae class. 
// i create 2 DriverData, one i use to write to the storage
// the text that's in "file". then i call "read" from the storage
// of 10 bytes less then the size of the file. i print to see the results
// then i "read" again, 10 chars from the beggining. 
//
//
// Have a file named "file" and put some more then 50 chars in it.
// Have another file for the DiskStorage named storage.dat


void print(char c)
{
	std::cout << c;
}

char copy(char c)
{
	return c;
}

bool DiskStorageTest()
{
	bool flag = true;

	// create a vector and copy the file to it
	std::vector<char> vec;
	char c;
	int fd = open("file", O_RDONLY);
	
	while(0 < read(fd, &c, 1))
	{
		vec.push_back(c);
	}
	
	// create a DriverData and copy the vector to it
	char handle[8] = "0110220";
	std::unique_ptr<DriverData> write_request(new DriverData(
	DriverData::WRITE, handle, 30, vec.size()));
	std::vector<char>& write_vec = write_request->access_buffer();
	
	std::transform(vec.begin(), vec.end(), write_vec.begin(), copy);
	
	// create DiskStorage and write the request to it
	DiskStorage storage("storage.dat" ,1000);
	std::unique_ptr<DriverData> ret = storage.write(std::move(write_request));
	
	// read from the storage, 10 less then the write
	std::unique_ptr<DriverData> 
	read_request(new DriverData(DriverData::READ, handle, 30, vec.size() - 10));
	std::unique_ptr<DriverData> ret_read = storage.read(std::move(read_request));
	std::vector<char>& read_vec = ret_read->access_buffer();
	
	std::for_each(read_vec.begin(), read_vec.end(), print);
	
	std::cout << "----------------" << std::endl;
	// read from 10 after the beggining
	std::unique_ptr<DriverData> 
	read_request2(new DriverData(DriverData::READ, handle, 40, vec.size() - 10));
	std::unique_ptr<DriverData> ret_read2 = storage.read(std::move(read_request2));
	std::vector<char>& read_vec2 = ret_read2->access_buffer();
	
	std::for_each(read_vec2.begin(), read_vec2.end(), print);
	
	return flag;
}


int main()
{
	RUN_TEST(DiskStorageTest);
	
	
	return 0;
}
