/***************************************************
	
	Exceptions
	
	types of exceptions used in the project

	Author: Yoav Klein
	Date: 8.12.19

****************************************************/


#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <stdexcept>
#include <iostream>

namespace ilrd
{

class DriverError : public std::runtime_error
{
public:
	DriverError(const std::string& orig, const char* msg);
	
	const std::string& who();

private:
	std::string m_orig;
};

class DiskError : public std::runtime_error
{
public:
	DiskError(const std::string msg);
};

// Exception types
class BadKey : public std::logic_error
{
public:
	BadKey(std::string str);
};

class BadCreate : public std::bad_alloc
{
public:
	BadCreate(); 
};



} // ilrd
#endif // __EXCEPTIONS_HPP__
