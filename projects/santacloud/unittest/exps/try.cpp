#include <iostream>
#include <vector>

int main() 
{
	std::vector<int> vec(1000);
	
	std::cout << vec.capacity() << std::endl;
	
	return 0;
}
