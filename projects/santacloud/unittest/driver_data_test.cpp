#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <algorithm>

#include "driver_data.hpp"
#include "tools.hpp"

#define CHECK(x, y) if(x != y) { flag = false; }

#define RUN_TEST(x) if(x() == true) { std::cout << GREEN "Test OK!" RESET << std::endl; }\
					else { std::cout << RED "Test Failed!" RESET << std::endl;} 

using namespace ilrd;

// this file should test the function of the class DriverData

// i create a vector and read from the file "file" in this directory
// then i create a DriverData instance, with length of the size of the vector
// then i copy the vector to the vector in the DriverData, 
// and check that everything went right

char Copy(char c)
{
	return c;
}

void Print(char c)
{
	std::cout << c;
}

int TestDriverData()
{
	bool flag = true;
	
	std::vector<char> vec;
	char handle[8] = "0123456";
	
	char c;
	int fd = open("file", O_RDONLY);
	
	while(0 < read(fd, &c, 1))
	{
		vec.push_back(c);
	}
	
	DriverData driver_data(DriverData::READ, handle, 40, vec.size() + 40);
	
	// test get_type
	CHECK(DriverData::READ, driver_data.get_type())
	
	// test get_handle
	CHECK(0, strcmp("0123456", driver_data.get_handle()))
	
	// test get_offset
	CHECK(40, driver_data.get_offset())
	
	//test get_length
	CHECK(driver_data.get_length(), vec.size() + 40)
	
	// write and read from vector
	
	std::size_t offset = driver_data.get_offset();
	std::vector<char>& buff = driver_data.access_buffer();
	
	std::transform(vec.begin(), vec.end(), buff.begin() + offset, Copy);
	
	// print vec
	std::for_each(vec.begin(), vec.end(), Print);
	
	// print driver_data.access_buffer()
	std::for_each(driver_data.access_buffer().begin(),
				  driver_data.access_buffer().end(),
				  Print);
	
	return flag;
}

int main()
{
	RUN_TEST(TestDriverData);
	
	return 0;
}



