
#include <iostream> // ostream

namespace ilrd
{

class Animal;
std::ostream& operator<<(std::ostream& os, const Animal& animal);
std::istream& operator>>(std::istream& is, Animal& animal);

class Animal
{
public:
	virtual std::ostream& serialize(std::ostream& stream) const;
	virtual std::istream& deserialize(std::istream& stream);
	
	void set_name(std::string name);
	void set_color(std::string color);
	void set_legs(int num_legs);
	
private:
	friend std::istream& operator>>(std::istream& is, Animal& animal);
	friend std::ostream& operator<<(std::ostream& os, const Animal& animal);
	std::string m_name;
	std::string m_color;
	int m_num_legs;
};

class Dog : public Animal
{
public:
	std::ostream& serialize(std::ostream& os) const override;
	std::istream& deserialize(std::istream& is) override;
	
	void set_type(std::string type);
	void set_favorite_food(std::string food);
	
private:
	std::string m_type;
	std::string m_favorite_food;
};

class Cat : public Animal
{
public:
	std::ostream& serialize(std::ostream& os) const override;
	std::istream& deserialize(std::istream& is) override;
	
	void set_type(std::string type);
	void set_vegi(bool is_vegi);
private:
	std::string m_type;
	bool m_is_vegetarian;
};

} // ilrd
