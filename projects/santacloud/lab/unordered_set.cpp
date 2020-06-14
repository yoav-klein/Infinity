#include <unordered_set>
#include <iostream>

class SomeClass
{
public:
	void speak()
	{
		std::cout << "hello" << std::endl;
	}

private:

};

int main()
{
	std::unordered_set<SomeClass> set;
	
	set.insert(SomeClass());
	set.insert(SomeClass());
	
	
	for(auto iter = set.begin(); iter != set.end(); ++iter)
	{
		iter->speak();
	}
	
	return 0;
}
