#include <iostream>

void Throw();
int main()
{
	try
	{
		Throw();
	}
	catch(std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}
