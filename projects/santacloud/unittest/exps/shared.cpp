#include <iostream>
#include <memory>

void Deleter(int* a)
{
	std::cout << "deleter: " << a << std::endl;
}

int main()
{
	std::shared_ptr<int> sp;
	
	
	sp.reset((int*)2, Deleter);
	
	return 0;
}
