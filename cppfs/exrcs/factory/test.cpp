#include <memory> // std::unique_ptr
#include <iostream> // std::cout
#include "../singleton/handleton/handleton.hpp" // Handleton..::get_instance()
#include "animals.hpp" // Animal's derived classes
#include "factory.hpp" // Factory<Animal, std::string, int>

using namespace ilrd;

// simulate a create_instance function of dog.
std::unique_ptr<Animal> dog_creator_simulator(int n)
{
	(void)n;
	std::cout << "this print is from a function that simulates a \
	function that replaces the Dog-get_instance function" << std::endl;
	
	return nullptr;
}


// simulate a create_instance function that takes three parameters
std::unique_ptr<int> int_creator(int a, int b, std::unique_ptr<int> ptr)
{
	std::unique_ptr<int> ptr_ret(new int(a + b + *ptr));
	
	return ptr_ret;
}

bool basic_test()
{
	bool flag = true;
	Factory<Animal, std::string, int>::add_type add_type;
	
	Factory<Animal, std::string, int>* animals_factory =
	Handleton<Factory<Animal, std::string, int> >::get_instance();
	
	animals_factory->add("Dog", Dog::create_instance);
	animals_factory->add("Cat", Cat::create_instance);
	animals_factory->add("Bird", Bird::create_instance);
	
	std::unique_ptr<Animal> dog = animals_factory->create("Dog",4);
	dog->make_sound();
	dog->print_legs();
	
	std::unique_ptr<Animal> dog2 = animals_factory->create("Dog", 1);
	dog2->make_sound();
	dog2->print_legs();
	
	std::unique_ptr<Animal> cat = animals_factory->create("Cat", 3);
	cat->make_sound();
	cat->print_legs();
	
	std::unique_ptr<Animal> bird = animals_factory->create("Bird", 2);
	bird->make_sound();
	bird->print_legs();
	
	// replace the function of Dog
	add_type = animals_factory->add("Dog", dog_creator_simulator);
	if(add_type != 1)
	{
		flag = false;
	}
	animals_factory->create("Dog", 2);
	
	// try to access a non-existing key
	try
	{
		animals_factory->create("Giraffe", 2);
	}
	catch(BadKey& err)
	{
		std::cout << "attempted to access a non-existing key" << std::endl;
		std::cout << err.what() << std::endl;
	}
	
	return flag;
}

void complex_test()
{
	Factory<int, int, int, int, std::unique_ptr<int> > int_factory;
	
	int_factory.add(1, int_creator);
	std::unique_ptr<int> ptr(new int(3));
	std::unique_ptr<int> a = int_factory.create(1, 4, 5, std::move(ptr));
}

int main()
{
	basic_test();	
	
	complex_test();
	
	return 0;
}
