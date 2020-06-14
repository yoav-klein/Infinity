#include <iostream>

int main()
{
	char c;
	while(1)
	{
		std::cout << "enter char" << std::endl;
		
		std::cin >> c;
		if(c == 'q')
		{
			continue;
		}
		std::cout << "not 'q'" << std::endl;
	
	}

}
