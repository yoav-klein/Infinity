#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>  // stew

void display(int line, const std::string& module, const std::string& msg)
{
	std::ostringstream string;
	string.fill('-');
	string << "[" << std::setw(3) << std::left << line << "]" << " ";
	string << std::setw(13) << std::left << module << ": ";
	string << std::setw(25) << std::left << msg << " "; 
	
	std::cout << string.str() << std::endl;
}

void string_formatting()
{
	display(2, "NBDDriver", "Problem");
	
	display(32, "main", "good");
	display(32, "main", "good");
	display(32, "main", "good");
	display(32, "main", "good");
}


int main()
{
	string_formatting();

	return 0;
}
