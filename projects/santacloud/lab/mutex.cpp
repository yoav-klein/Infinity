#include <mutex>
#include <vector>
#include <iostream>
int main()
{
	std::vector<std::mutex> vec(1);
	
	vec[10].lock();
	
	std::cout << "LOCKED" << std::endl;
	vec[10].unlock();
}
