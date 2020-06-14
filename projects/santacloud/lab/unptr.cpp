#include <iostream>
#include <memory>

void bar(const std::unique_ptr<int>& ptr)
{
	std::cout << *ptr << std::endl;
}


void foo(std::unique_ptr<int> ptr)
{
	bar(ptr);
}

int main()
{
	std::unique_ptr<int> my_ptr(new int(44));
	//foo(std::move(my_ptr));
	foo(std::unique_ptr<int>(new int(22)));
	
	return 0;
}
