#include <memory>
#include <iostream>

class SomeClass
{
public:
	SomeClass() { std::cout << "ctor" << std::endl; }
	~SomeClass() { std::cout << "dtor" << std::endl; }

};

template <typename T>
void fifi()
{
	//SomeClass();
	static std::unique_ptr<SomeClass> uptr(new SomeClass());
	std::cout << "uptr" << &uptr << std::endl;

}
