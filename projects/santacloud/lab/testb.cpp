 #include <unistd.h>
 #include <sys/types.h>
#include <iostream>
#include <fstream>

 #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>


class Storage
{
public:
	Storage(std::string name, std::size_t size);
	~Storage();
	void write(const char *buff, std::size_t offset, std::size_t size);
	void read(char *buff, std::size_t offset, std::size_t size);
	
	
private:
	int m_fd;
};

void read_all(int fd, char* buff, std::size_t size);
void write_all(int fd, const char *buff, std::size_t size);

Storage::Storage(std::string name, std::size_t size)
{
	m_fd = open(name.c_str(), O_RDWR);
	if(-1 == m_fd)
	{
		perror("open:");
		exit(1);
	}
	if(-1 == truncate(name.c_str(), size))
	{
		perror("truncate");
	}
}

Storage::~Storage()
{
	close(m_fd);
	
}

void Storage::write(const char *buff, std::size_t offset, std::size_t size)
{
	lseek(m_fd, offset, SEEK_SET);
	write_all(m_fd, buff, size);
}


void write_all(int fd, const char *buff, std::size_t size)
{
	int bytes_written;
	
	while(size > 0)
	{
		bytes_written = write(fd, buff, size);
		buff += bytes_written;
		size -= bytes_written;
	}	
}


void Storage::read(char *buff, std::size_t offset, std::size_t size)
{
	lseek(m_fd, offset, SEEK_SET);
	read_all(m_fd, buff,  size);
}

void read_all(int fd, char* buff, std::size_t size)
{
	int bytes_read = 0;
	
	while(size > 0)
	{
		bytes_read = read(fd, buff, size);
		buff = buff + bytes_read;
		size -= bytes_read;
	}
	
	buff[bytes_read] = '\0';
	std::cout << buff;
	
	//delete[] buff;
}

int main()
{
	Storage s("file.dat", 1024);
	
	
	s.write("YoavKlein", 30, 9);
	//s.write("AA", 60, 2);
	//s.write("D", 10, 1);
	//s.read(30, 4);
	char buff[9];
	s.read(buff, 30, 9);
	
	std::cout << buff;
	
	return 0;
}




