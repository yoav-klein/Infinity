/**
 *  Project: factory.hpp
 *  Date: 18/11/2019
 
 *  Factory class handles creation of objects of diffrent types of
    the same heirarchy as required at runtime.
 
 *  Creator class sepcifies the creation function needed by each 
    diffrent concrete factory class.

 *  The derived classes must provide a static func that follows
    the signature of the creator func

 *  add() --> adds the <key, create_func> to the unordered_map.
    returns ADDED on success of new key insertion, if the key existsd REPLACED 
    will be returned.  
    on failure it throws  BadAdd (you shoul implement your own BadAdd class 
    that inherits from std::runtime_error)

 *  create() --> returns a pointer to newly allocated object.
    On failure it throws exception of incorrect key or bad allocation
    (you should implement your own BadKey which inherits from std::logic_error
    and BadCreate which inherits from std::runtime_error).


 * Notice that the user must provide some arguments to the create() function
  	according to the signature of the static function it initiates.
*/

#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <functional> // std::function
#include <unordered_map> // std::unordered_map
#include <memory> // std::unique_ptr

#include "handleton.hpp"

namespace ilrd
{

// Exception types
class BadKey : public std::logic_error
{
public:
	BadKey(std::string str)
		: std::logic_error(str) { }
};

class BadCreate : public std::bad_alloc
{
public:
	BadCreate()
		: std::bad_alloc() { } 
};


template <typename Base, typename Key, typename... Args>
class Factory
{
//add comments on exception safety
public:
    enum add_type
    {
        ADDED,
        UPDATED
    };
	
    using CreatorFunc = std::function<std::unique_ptr<Base>(Args... args)>; 
	
	Factory() = default;
    add_type add(const Key& key, CreatorFunc creator);

    //runtime_error::bad_alloc, logical error(out of range)
    //define 2 classes: Bad_Key, Bad_Create : inherit from these exceptions
    std::unique_ptr<Base> create(const Key& key, Args... args) const;

    ~Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;
    Factory(Factory&&) = delete;
    Factory& operator=(Factory&&) = delete;

private:
    friend Handleton<Factory<Base,Key,Args...>>;
    std::unordered_map<Key, CreatorFunc> m_creators;
   

};

template <typename Base, typename Key, typename... Args>
typename Factory<Base,Key,Args...>::add_type 
		Factory<Base, Key, Args...>::add(const Key& key, CreatorFunc creator)
{
	// insert function of an unordered map returns a std::pair in which 
	// the second field is a bool indicating whether the insert succeeded 
	// or not, meaning that the key exists.
	bool not_exist = true;
	try
	{
		//not_exist = m_creators.insert({key, creator}).second; 
	}
	catch(std::bad_alloc& ba) 
	{
		throw BadCreate();
	}	
	if(!not_exist) // key exists
	{
		CreatorFunc& func_ref = m_creators.at(key);
		func_ref = creator;
		return UPDATED;
	}
	
	return ADDED;
}

template <typename Base, typename Key, typename... Args>
std::unique_ptr<Base> 
	Factory<Base, Key, Args...>::create(const Key& key, Args... args) const
{
	CreatorFunc creator;
	try
	{
		creator = m_creators.at(key);
	}
	catch(std::logic_error& le)
	{
		throw BadKey(le.what());
	}
	return creator(std::forward<Args>(args)...);
}

} //ilrd

#endif //__FACTORY_H__
