#include <iostream>
#include "scope_guard.h"

class Mutex
{
public:
	void Lock()
	{
		is_locked = true;
		std::cout << "Acquire some resource" << std::endl;
	}
	
	void Unlock()
	{
		is_locked = false;
		std::cout << "Release some resource" << std::endl;
	}

private:
	bool is_locked;
};

void MutextTest()
{
	Mutex m;
	
	ScopeGuard<Mutex, &Mutex::Lock, &Mutex::Unlock> guard(m);
}

int main()
{
		
	return 0;
}
