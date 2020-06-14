

#include <unordered_map>
#include <iostream>
#include <thread>


void map_basics()
{
	std::unordered_map<int, std::pair<char, char> > map(6);	
	
	std::cout << "max_size: " << map.max_size() << std::endl;
	std::cout << "bucket count: " << map.bucket_count() << std::endl;
	
	for(int i = 0; i < 4; ++i)
	{
		map.emplace(i, std::pair<char, char>('c', 'y'));
		std::cout << "size: " << map.size() << std::endl;
	}
	
	for(auto i : map)
	{
		std::cout << i.first << " " << i.second.first << std::endl;
	}
}

void foo()
{

}	

void map_threads()
{
	std::unordered_map<std::thread::id, std::pair<std::thread, bool> > map(4);	
	
	for(int i = 0; i < 5; ++i)
	{
		std::cout << i << std::endl;
		std::thread thread(foo);
		
	//	std::pair<std::thread, bool> content(std::move(thread), true);
		
	//	std::pair<std::thread::id, std::pair<std::thread, bool>> 
	//	pair(thread.get_id(), std::move(content)); 
	//	map.emplace(std::move(pair));
		
		std::thread::id id = thread.get_id();
		map.emplace(std::make_pair(id,
						std::make_pair(std::move(thread), true)));
		
	}
	
	for(auto& i : map)
	{
		i.second.first.join();
		//map.erase(i.first);
	}
	
	for(auto& i : map)
	{
		map.erase(i.first);
	}
	
	
	std::cout << map.size() << std::endl;
}

int main()
{
	map_threads();
	
	return 0;
}
