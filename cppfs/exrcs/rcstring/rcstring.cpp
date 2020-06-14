#include <iostream> // size_t
#include <cstring> // strlen
#include <algorithm> // swap

#include "rcstring.h"

namespace ilrd
{

const std::size_t counter_size = sizeof(std::size_t);

RCString::RCString(const char *str)
	: m_buffer(InitBuffer(str))
{
}

RCString::RCString(const RCString& other)
	: m_buffer(other.m_buffer)
{
	++*(reinterpret_cast<std::size_t*>(m_buffer));
}

RCString::~RCString()
{
	DeleteIfShould();
}

RCString& RCString::operator=(const RCString& other)
{
	RCString temp(other);	
	
	std::swap<char*>(m_buffer, temp.m_buffer);
	
	return *this;
}

void RCString::DeleteIfShould()
{
	std::size_t* counter = reinterpret_cast<std::size_t*>(m_buffer);
	
	--*counter;
	if(!*counter)
	{
		delete[] m_buffer;
	}
}

char* RCString::InitBuffer(const char* str)
{
	char* ret = new char[strlen(str) + counter_size + 1];
	*(reinterpret_cast<std::size_t*>(ret)) = 1;
	
	strcpy(ret + counter_size, str);
	return ret;
}

char& RCString::operator[](std::size_t index)
{
	if(IsShared())
	{
		--*(reinterpret_cast<std::size_t*>(m_buffer));
		m_buffer = InitBuffer(m_buffer + counter_size);
	}
	return m_buffer[index + counter_size];
}

const char& RCString::operator[](std::size_t index) const
{
	return m_buffer[index + counter_size];
}

std::size_t RCString::Length() const
{
	return strlen(ToCStr());
}

const char* RCString::ToCStr() const
{
	
	return (m_buffer + counter_size);
}

bool RCString::IsShared() const
{
	return (*(reinterpret_cast<std::size_t*>(m_buffer)) > 1);
}

// non-member functions

std::ostream& operator<<(std::ostream& os, const RCString& str)
{
	return os << str.ToCStr();
}

std::istream& operator>>(std::istream& is, RCString& str)
{
	std::string s;
	std::getline(is, s);
	
	str = RCString(s.c_str());
	
	return is;
}

bool operator==(const RCString& lhs, const RCString& rhs)
{
	return (0 == strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

bool operator<(const RCString& lhs, const RCString& rhs)
{
	return (0 > strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

bool operator>(const RCString& lhs, const RCString& rhs)
{
	return (0 < strcmp(lhs.ToCStr(), rhs.ToCStr()));
}

bool operator!=(const RCString& lhs, const RCString& rhs)
{
	return !(lhs == rhs);
}

} // namespace ilrd

