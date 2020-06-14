 #include <unistd.h>
 #include <sys/types.h>
#include <iostream>
#include <fstream>

#include <chrono>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <thread>

class Storage
{
public:
	Storage(std::string name, std::size_t size);

	void write(const char *buff, std::size_t size, std::size_t offset);
	void read(std::size_t size, std::size_t offset);
private:
	std::fstream file;
};

Storage::Storage(std::string name, std::size_t size)
{
	file.open(name.c_str(), std::fstream::in | std::fstream::out);
	if(-1 == truncate(name.c_str(), size))
	{
		perror("truncate");
	}
}

void Storage::read(std::size_t size, std::size_t offset)
{
	char buff[10];
	
	file.seekg(offset);
	file.read(buff, size);
	
	std::cout << buff;
}

void Storage::write(const char *buff, std::size_t size, std::size_t offset)
{
	file.seekp(offset);
	file.write(buff, size);
}

void files()
{
	char* buffer = new char[5];
	int fd = open("file.txt", O_WRONLY);
	if(-1 == truncate("file.txt", 1000))
	{
		perror("truncate");
	}
	
	close(fd);
	/*
	std::ifstream infile;
	infile.open("file.txt");
	infile.seekg(pos - 4);
	infile.read(buffer, 4);
	
	std::cout << buffer;
*/}

void openagain()
{
	int fd = open("file.txt", O_WRONLY);
	
	lseek(fd, 30, SEEK_SET);
	write(fd, "YoavKlein", 10);
}

void concurrency()
{
	std::cout << std::thread::hardware_concurrency();
}

void chrono()
{
	std::chrono::duration<int, std::ratio<1>> seconds(3);
}

int main()
{
	//Storage s("yoav.dat", 5000);
	
//	chrono();
	concurrency();
	//s.write("Yoav", 4, 30);
	//s.write("AA", 5, 60);
	//s.write("D", 1, 10);
	//s.read(4, 30);
	//s.read(5, 60);
	
	return 0;
}




