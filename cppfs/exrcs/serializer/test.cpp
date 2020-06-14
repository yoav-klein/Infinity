/*

*	Serializer test
* 
*	there is an Animal class as base, Dog and Cat derives,
*	i create a Serializer<Animal>
*	create a Dog and a Cat, set their values,  and serialize them
*	into a file "animals.txt". then deserialize from the file 
*	into unique_ptrs and print them. expect to see the same values

*/



#include <fstream> // ofstream
#include <iostream> // ostream
#include "serializer.hpp"
#include "animals.hpp"

using namespace ilrd;

void serialize(Serializer<Animal>& ser)
{
	std::ofstream file("animals.txt");
	
	Dog dog;
	dog.set_name("Bob");
	dog.set_color("Brown");
	dog.set_legs(3);
	dog.set_type("Pointer");
	dog.set_favorite_food("Spaghetti");
	
	ser.serialize(dog, file);

	Cat cat;
	cat.set_name("Mizi");
	cat.set_color("Blue");
	cat.set_legs(4);
	cat.set_type("Persian");
	cat.set_vegi(true);
	
	ser.serialize(cat, file);
}

void deserialize(Serializer<Animal>& ser)
{
	std::ifstream file("animals.txt");
	
	std::unique_ptr<Animal> ret_dog = ser.deserialize(file);
	std::cout << *ret_dog;
	
	std::unique_ptr<Animal> ret_cat = ser.deserialize(file);
	std::cout << *ret_cat;
}

void test_serializer()
{
	Serializer<Animal> ser;
	ser.add<Dog>();
	ser.add<Cat>();
	
	serialize(ser);
	
	deserialize(ser);
	
}

int main()
{	
	test_serializer();
	
	return 0;
}
