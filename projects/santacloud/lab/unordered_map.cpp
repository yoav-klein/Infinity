#include <iostream>
#include <unordered_map>

// playing with unordered map. adding three elements to a map 
// and using 'find'.
// a simulation for DllLoader

void just_find()
{
	std::unordered_map<int, std::string> map; 
	
	map.insert({1, "One"});
	map.insert({2, "Two"});
	map.insert({3, "Three"});
	
	char c = '0';
	while(c != 'q')
	{

		int key;
		std::cout << "enter key" << std::endl;
		std::cin >> key;
		
		std::unordered_map<int, std::string>::iterator iter = map.find(key);
		
		if(iter != map.end())
		{
			std::cout << (*iter).first << std::endl;
			std::cout << (*iter).second << std::endl;
		}
		else
		{
			std::cout << "not found!" << std::endl;
		}
		
		std::cout << "new search" << std::endl;
		std::cin >> c;
	}
	
}

void find_and_erase()
{
	std::unordered_map<int, std::string> map; 
	
	map.insert({1, "One"});
	map.insert({2, "Two"});
	map.insert({3, "Three"});
	
	char c = '0';
	while(c != 'q')
	{
		int key;
		std::cout << "enter key" << std::endl;
		std::cin >> key;
		
		std::unordered_map<int, std::string>::iterator iter = map.find(key);
		
		if(iter != map.end())
		{
			std::cout << (*iter).first << std::endl;
			std::cout << (*iter).second << std::endl;
			map.erase(iter);
			std::cout << "size: " << map.size() << std::endl;
		}
		else
		{
			std::cout << "not found!" << std::endl;
		}
		
		std::cout << "new search" << std::endl;
		std::cin >> c;
	}
	
}

void iterating()
{
	std::unordered_map<int, std::string> map; 
	
	map.insert({1, "One"});
	map.insert({2, "Two"});
	map.insert({3, "Three"});
	
	for(auto i : map)
	{
		std::cout << i.first << std::endl;
	}
}

int main()
{
	iterating();

}
