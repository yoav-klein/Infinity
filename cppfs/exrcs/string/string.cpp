/****************************************
*		String
*		Author: Yoav Klein
*		Date: 18.9.19
******************************************/

#include <cstring> // strcmp

#include "string.h"

char* String::Strdup(const char* str_literal)
{
	size_t length = strlen(str_literal);
	char *new_string = new char[length + 1];
	strcpy(new_string, str_literal);
	
	return new_string;
}

String::String(const char *str) : m_string(Strdup(str))
{}

String::String(const String& s1) : m_string(Strdup(s1.m_string))
{}

String::~String()
{
	delete[] m_string;
}

String& String::operator=(const String& other)
{
	if(other.m_string != m_string)
	{
		delete[] m_string;
		m_string = Strdup(other.m_string);
	}
	
	return *this;
}

size_t String::Length() const
{
	return strlen(m_string);
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
	os << str.m_string;
	
	return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
	delete[] m_string;
	
	char buffer[50];
	is.getline(buffer, 50);
	
	str = buffer;
	return is;
}

const char *String::ToCStr() const
{
	return m_string;
}

bool String::operator>(const String& other) const
{
	return (strcmp(m_string, other.m_string) > 0);
}

bool String::operator<(const String& other) const
{
	return (strcmp(m_string, other.m_string) < 0);
}

char& String::operator[](size_t index)
{
	return m_string[index];
}

const char& String::operator[](size_t index) const
{
	return m_string[index];
}

String& String::Concat(const String& other)
{	
	char *buffer = new char[strlen(m_string) + strlen(other.m_string) + 1];
	strcpy(buffer, m_string);
	strcat(buffer, other.m_string);
	delete[] m_string;
	m_string = buffer;
	
	return *this;
}

bool operator==(const String& first, const String& second)
{
	return !strcmp(first.m_string, second.m_string);
}

bool operator!=(const String& first, const String& second)
{
	return !(first == second);
}


