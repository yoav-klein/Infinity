
/**
 *  Project: serializer.hpp
 *  Date: 19/11/2019
 
 *  Serializer class handles serialization and desirialization of objects of 
    diffrent types of the same heirarchy as required at runtime.

 *  The derived classes must provide an overload to operator<< and operator>>.

 *  add() --> adds the ability to serialize and deserialize a specific Derived
    type. Any type the class should be able to handle must first be added to it.
    On failure it throws exception of bad allocation: BadAdd, which inherits
    from std::runtime_error.

 *  deserialize() --> returns a pointer to Base holding newly allocated Derived 
    object.
    On failure it throws exception of incorrect key: BadKey which inherits 
    from std::logic_error or bad allocation: BadAdd, which inherits
    from std::runtime_error.


 *  the user should provide the overloads of operators >> and  <<, 
   	that in turn invocates virtual functions "serialize" and "deserialize",
   	that will be implemented in each derived class. 
*/

#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

//#include <iostream>

#include "../factory/factory.hpp"


namespace ilrd
{

template <typename Base>
class Serializer
{ 
public:
    Serializer() = default;
    ~Serializer() = default;

    Serializer(const Serializer&) = delete;
    Serializer& operator=(const Serializer&) = delete;
    Serializer(Serializer&&) = delete;
    Serializer& operator=(Serializer&&) = delete;

    template<typename Derived>
    void add();

    void serialize(const Base& obj, std::ostream& stream) const;
    std::unique_ptr<Base> deserialize(std::istream& stream) const;

    template<typename Derived>
    static std::unique_ptr<Base> creator(std::istream& stream);

private:
    Factory<Base, std::size_t, std::istream&> m_factory;

};

template <typename Base>
template <typename Derived>
void Serializer<Base>::add()
{
	m_factory.add(typeid(Derived).hash_code(), creator<Derived>);
}

template <typename Base>
void Serializer<Base>::serialize(const Base& obj, std::ostream& stream) const
{
	stream << typeid(obj).hash_code();
	stream << obj;
}

template <typename Base>
std::unique_ptr<Base> Serializer<Base>::deserialize(std::istream& stream) const
{
	std::size_t type_code;
	stream >> type_code;
	
	return m_factory.create(type_code, stream);
}

template <typename Base>
template <typename Derived>
std::unique_ptr<Base> Serializer<Base>::creator(std::istream& stream)
{
	std::unique_ptr<Derived> t(new Derived());
	stream >> *t;
	
	return t;
}

}//ilrd

#endif //__SERIALIZER_H__
