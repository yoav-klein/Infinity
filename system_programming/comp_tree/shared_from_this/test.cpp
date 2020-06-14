#include <memory>
#include <iostream>

class Cat : public std::enable_shared_from_this<Cat>
{
public:
	Cat() { std::cout << "Cat Ctor" << std::endl; }
	~Cat() { std::cout << "Cat Dtor" << std::endl; }
	
	std::shared_ptr<Cat> getptr()
	{
		return shared_from_this();
	}
};

class Dog
{
public:	
	Dog() { std::cout << "Dog Ctor" << std::endl; }
	~Dog() { std::cout << "Dog Dtor" << std::endl; }
	
	std::shared_ptr<Dog> getptr()
	{
		return std::shared_ptr<Dog>(this);
	}
};

void DogEx()
{
	std::shared_ptr<Dog> sp_dog(new Dog());
	std::cout << sp_dog.use_count() << std::endl;
	
	std::shared_ptr<Dog> sp_dog2(sp_dog->getptr());
	
	std::cout << sp_dog.use_count() << std::endl;
	
}

void CatEx()
{
	std::shared_ptr<Cat> sp_cat = std::make_shared<Cat>();
	std::cout << sp_cat.use_count() << std::endl;
	
	Cat* ptr = sp_cat.get();
	std::shared_ptr<Cat> sp_cat2(ptr->getptr());
	
	std::cout << sp_cat.use_count() << std::endl;
}

void CatNotGood()
{ // shared_from_this can be used only if one object was created with a shared_ptr before!
	Cat* ptr_cat = new Cat();
	
	std::shared_ptr<Cat> sp_cat(ptr_cat->getptr());
	
	std::cout << sp_cat.use_count() << std::endl;
}

std::shared_ptr<int> IntSp()
{
	std::shared_ptr<int> sp(new int(2));
	return 0;
}

int main()
{

	//DogEx()
	
	//CatEx();
	CatNotGood();
	
	//std::shared_ptr<int> ptr = IntSp();
	//std::cout << ptr;
	
	return 0;
}
