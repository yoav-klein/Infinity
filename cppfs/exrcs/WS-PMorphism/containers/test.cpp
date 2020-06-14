#include <iostream>
#include <list>

using namespace std;


int main() 
{
	std::list<int>::iterator iter_first;
	std::list<int>::iterator iter_last;
	
	std::list<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(4);
	list.push_back(6);
	list.push_back(22);
	list.push_back(31);
	
	iter_first = list.begin();
	
	while(iter_first != list.end())
	{
		std::cout << *iter_first << " ";
		std::advance(iter_first, 1);
	}
	
	return 0;
}


