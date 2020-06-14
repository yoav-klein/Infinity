/****************************************
*		String
*		Author: Yoav Klein
*		Date: 18.9.19
******************************************/

#include <cstring> // strcmp

#include "string.h"

void String::Construct(const char* str_literal)
{
	size_t length = strlen(str_literal);
	string = new char[length + 1];
	strcpy(string, str_literal);
}

String::String(const char *str)
{
	Construct(str);
}

String::String(const String& s1)
{
	Construct(s1.string);
}

String::~String()
{
	delete[] string;
}

String& String::operator=(const String& other)
{
	return *this = other.string;
}

String& String::operator=(const char *str_literal)
{
	delete[] string;
	Construct(str_literal);
	
	return *this;	
}

size_t String::Length() const
{
	return strlen(this->string);
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
	os << str.string;
	
	return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
	char buffer[50];
	
	is >> buffer;
	
	str = buffer;
	
	return is;
}

bool String::operator==(const String& other) const
{
	return (*this == other.string);
}

char *String::ToCStr() const
{
	return string;
}

bool String::operator!=(const String& other) const
{
	return !(other.string == *this);
}

bool String::operator==(const char* str_literal) const
{
	return (str_literal == *this);
}

bool operator==(const char* str_literal, const String& str)
{
	return !(strcmp(str.ToCStr(), str_literal));
}

bool String::operator>(const String& other) const
{
	return (strcmp(string, other.string) > 0);
}

bool String::operator<(const String& other) const
{
	return (strcmp(string, other.string) < 0);
}

char& String::operator[](size_t index) const
{
	return string[index];
}

String& String::Concat(const String& other)
{	
	char *buffer = new char[strlen(string) + strlen(other.string) + 1];
	strcpy(buffer, string);
	strcat(buffer, other.string);
	delete[] string;
	string = buffer;
	
	return *this;
}



