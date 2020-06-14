#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


void write()
{
	std::ofstream out_file("streams.txt");
	
	std::size_t st = 12345678901234567890ul;
	int n = 433;
	
	out_file << st;
	out_file << ";";
	out_file << n;
	
	out_file.close();

}

void read()
{
	std::ifstream in_file("streams.txt");
	char c;
	
	std::size_t offset;
	in_file >> offset;
	
	in_file >> c;	
		
	int length;
	in_file >> length;
	
	in_file >> c;
	
	int req_id;
	in_file >> req_id;
	
	in_file >> c;
	
	int req_type;
	in_file >> req_type;
	
	std::cout << offset << " " << length << " " << req_id << " " << req_type << std::endl;
}

void byte_by_byte_fopen()
{
	FILE* file = fopen("streams.txt", "r");
	if(NULL == file)
	{
		perror("fopen");
		exit(1);
	}
	
	char c = fgetc(file);
	std::cout << c;
	
	while(c == fgetc(file))
	{
		std::cout << 1;
		std::cout << c;	
	}
	
	fclose(file);
}

void byte_by_byte(int fd)
{
	
	std::string str;
	char c;
	read(fd, &c, 1);
	while(c != '\0')
	{
		str += c;
		read(fd, &c, 1);
	}
	
	std::cout << str << std::endl;
	
	std::size_t delim = str.find_first_of(";");
	std::size_t last;
	
	std::string offset_str = str.substr(0, delim);
	std::size_t offset = atol(str.c_str());
	std::cout << "offset: " << offset << std::endl;
	
	last = delim + 1;
	str[delim] = 'a';
	
	delim = str.find_first_of(";");
	std::string length_str = str.substr(last, delim);
	int length = atoi(length_str.c_str());
	std::cout << "length: " << length << std::endl; 
	str[delim] = 'a';
	
	last = delim + 1;
	delim = str.find_first_of(";");
	std::string req_id_str = str.substr(last, delim);
	int req_id = atoi(req_id_str.c_str());
	std::cout << "req_id: " << req_id << std::endl;
	
	str[delim] = 'a';
	last = delim + 1;
	delim = str.find_first_of(";");
	std::string req_type_str = str.substr(last, delim);
	int req_type = atoi(req_type_str.c_str());
	std::cout << "req_type: " << req_type << std::endl;
	
}

void write_fd()
{
	int fd = open("streams.txt", O_WRONLY);
	
	dprintf(fd, "%li;%d;%d;%d;%c", 98765432100, 110, 3, 1, 0);
	dprintf(fd, "%li;%d;%d;%d;%c", 98765432122, 220, 5, 0, 0);
}

void sscanf(int fd)
{
	char buffer[80];
	
	read(fd, buffer, 80);

	std::size_t offset;
	int length;
	int req_id;
	int req_type;
	
	sscanf(buffer, "%lu;%d;%d;%d", &offset, &length, &req_id, &req_type);
	
	std::cout << offset <<  " " << length << " "  << req_id << " " << req_type;
}

void strtok(int fd)
{
	char buffer[80];
	
	read(fd, buffer, 80);
	
	/*for(int i = 0; i < 80; ++i)
	{
		if(*(buffer + i) == '\0')
		{
			std::cout << "*";
		}
		else
		{
			std::cout << *(buffer + i);
		}
	}*/
	std::cout << std::endl;
	
	char *curr = buffer;
	std::cout << static_cast<void*>(curr) << std::endl;
	while(*curr != '\0')
	{
		++curr;
	}
	++curr;
	
	std::cout << static_cast<void*>(curr) << std::endl;
	std::cout << curr + 1 << std::endl;
	
	/*for(int i = 0; i < 40; ++i)
	{
		std::cout << *(curr + i);
	}*/
}

int main()
{
	//write_fd();
	
	int fd = open("streams.txt", O_RDONLY);
	
	
	//byte_by_byte(fd);
	//byte_by_byte(fd);
	//sscanf(fd);
	strtok(fd);
	
	close(fd);

	return 0;
}
