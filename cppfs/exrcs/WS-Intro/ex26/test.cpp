#include <iostream>


int main()
{
	int i = 78;
	double d = 2.3;
	
	std::cout << "hello" << i << std::endl;
	std::cin >> i;
	std::cerr << "wrong?" << i << " " << d;
	
	i = i >> 1;
	
	std::cout << i;	
		
	return 0;
}
