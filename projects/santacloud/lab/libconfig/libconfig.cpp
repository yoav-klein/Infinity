#include <libconfig.h++>
#include <iostream>
//#include <string>

int main()
{
	libconfig::Config cfg;
	
	cfg.readFile("exmp.cfg");
	
	std::string s = cfg.lookup("name");
	
	std::cout << s << std::endl;
	

return 0;
}
