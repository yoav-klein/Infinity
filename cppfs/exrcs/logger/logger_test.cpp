#include <fstream> // ifstream
#include <sstream> // ostringstream
#include <iostream> // cout

#include "logger.hpp"
//#include "colors.hpp"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }


using namespace ilrd;


int TestLogger()
{
	int flag = 1;
	
	std::ofstream file("log.txt");
	Logger* logger = Handleton<Logger>::get_instance();
	//Logger* logger = new Logger();
	
	logger->add_output(file);
	logger->write(Logger::ERROR, "NBDDriver", "receiving request: read", __LINE__);
	logger->write(Logger::ERROR, "RAMStorage", "read request done", __LINE__);
	logger->write(Logger::ERROR, "main", "passing to storage", __LINE__);
	logger->write(Logger::ERROR, "NBDDriver", "receiving request: read", __LINE__);
	
	return flag;
}

int main()
{	
	RUN_TEST(TestLogger);
	//Try();
	
	return 0;
}
