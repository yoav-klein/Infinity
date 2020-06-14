
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <unistd.h>

class Animal
{
public:
	virtual void Foo() { }
};

class Dog : public Animal
{
public:
	void Foo() override { }

};

void which_type(Animal& t)
{
	std::cout << typeid(t).hash_code() << std::endl;
}

void read()
{
	
	std::ifstream in("file.txt");
	
	std::string res;
	in >> res;
	
	std::cout << res;
}

void write()
{
	std::ofstream out("file.txt");
	
	std::string str = "Yoavklein";
	
	out << str << "Yoav" << 2 << 9;
	
	
}

int main()
{	
	Dog d;
	Animal a;
	std::cout << typeid(d).hash_code() << std::endl;
	std::cout << typeid(a).hash_code() << std::endl;
	
	which_type(d);
	which_type(a);
	
	write();
	read();
	
	return 0;
}
