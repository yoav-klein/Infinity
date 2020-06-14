#include <iostream> // std::cout
#include <memory> // std::unique_ptr

#include "animals.hpp"

namespace ilrd
{

// ctors

Animal::Animal(int num_legs)
	: m_num_legs(num_legs) { }
	
void Animal::print_legs()
{
	std::cout << "I have: " << m_num_legs << " legs" << std::endl;
}

Dog::Dog(int num_legs)
	: Animal(num_legs)
{

}

Cat::Cat(int num_legs)
	: Animal(num_legs)
{

}

Bird::Bird(int num_legs)
	: Animal(num_legs)
{

}


std::unique_ptr<Animal> Dog::create_instance(int n)
{
	
	return std::unique_ptr<Animal>(new Dog(n));
}

std::unique_ptr<Animal> Cat::create_instance(int n)
{
	return std::unique_ptr<Animal>(new Cat(n));
}

std::unique_ptr<Animal> Bird::create_instance(int n)
{
	return std::unique_ptr<Animal>(new Bird(n));
}

void Dog::make_sound()
{
	std::cout << "Woof!" << std::endl;
}


void Cat::make_sound()
{
	std::cout << "Miaou" << std::endl;
}


void Bird::make_sound()
{
	std::cout << "Zifzif" << std::endl;
}

} // namespace ilrd
