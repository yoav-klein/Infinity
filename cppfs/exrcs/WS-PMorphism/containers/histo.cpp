#include <iostream>
#include <list>

typedef std::pair<std::string, int> my_pair;

int main() 
{
	std::list<my_pair> histo;
	std::string input = std::string();
	std::list<my_pair>::iterator iter = histo.begin();
	
	std::cin >> input;
	while("." != input)
	{
		iter = histo.begin();
		while(iter != histo.end() && input != (*iter).first)
		{
			++iter;
		}
		if(iter != histo.end())
		{
			++(*iter).second;
		}
		else
		{
			histo.push_back(my_pair(input, 1));
		}
		std::cin >> input;
	}
	
	iter = histo.begin();
	while(iter != histo.end())
	{
		std::cout << (*iter).first << " -> " << (*iter).second << std::endl;
		++iter;
	}
	
	return 0;
}


