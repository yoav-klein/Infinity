#include <iostream> // ostream
#include "animals.hpp"


namespace ilrd
{

std::ostream& operator<<(std::ostream& os, const Animal& animal)
{
	return animal.serialize(os);
}

std::istream& operator>>(std::istream& is, Animal& animal)
{
	return animal.deserialize(is);
}	


void Animal::set_name(std::string name)
{
	m_name = name;
}

void Animal::set_color(std::string color)
{
	m_color = color;
}

void Animal::set_legs(int num_legs)
{
	m_num_legs = num_legs;
}

void Dog::set_type(std::string type)
{
	m_type = type;
}

void Dog::set_favorite_food(std::string food)
{
	m_favorite_food = food;
}

void Cat::set_type(std::string type)
{
	m_type = type;
}

void Cat::set_vegi(bool is_vegi)
{
	m_is_vegetarian = is_vegi;
}

std::ostream& Animal::serialize(std::ostream& os) const
{
	return os << m_name << "\n" << m_color << "\n" << m_num_legs;
}

std::istream& Animal::deserialize(std::istream& is)
{
	return is >> m_name >> m_color >> m_num_legs;
}

std::ostream& Dog::serialize(std::ostream& os) const
{
	return Animal::serialize(os) << m_type << "\n" << m_favorite_food << "\n";
	
}

std::istream& Dog::deserialize(std::istream& is)
{
	return Animal::deserialize(is) >> m_type >> m_favorite_food;
}


std::ostream& Cat::serialize(std::ostream& os) const
{
	return Animal::serialize(os) << m_type << "\n" << m_is_vegetarian << "\n";
}

std::istream& Cat::deserialize(std::istream& is)
{
	return Animal::deserialize(is) >> m_type >> m_is_vegetarian;
}



}
