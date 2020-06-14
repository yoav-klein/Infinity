
#include <memory> // std::unique_ptr

namespace ilrd
{

class Animal
{
public:
	Animal(int num_legs = 1);
	virtual void make_sound() = 0;
	void print_legs();
private:
	int m_num_legs;
};

class Dog : public Animal
{
public:
	static std::unique_ptr<Animal> create_instance(int n);
	void make_sound() override;
	
private:
	Dog(int num_legs);
};

class Cat : public Animal
{
public:
	static std::unique_ptr<Animal> create_instance(int n);
	void make_sound() override;
	
private:
	Cat(int num_legs);
};

class Bird : public Animal
{
public:
	static std::unique_ptr<Animal> create_instance(int n);
	void make_sound() override;
	
private:
	Bird(int num_legs);
};


} // ilrd
