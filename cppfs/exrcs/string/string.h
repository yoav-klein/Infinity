#include <cstddef> // size_t
#include <iostream>

class String
{
public:
	String(const char *str = "");
	String(const String& s1);
	~String();
	
	String& operator=(const String& other);
	bool operator>(const String& other) const;
	bool operator<(const String& other) const;
	char& operator[](size_t index);
	const char& operator[](size_t index) const;
	
	const char* ToCStr() const;
	String& Concat(const String& other);
	size_t Length() const;
	
private:
	char *m_string;
	char* Strdup(const char* str_literal);
	friend std::ostream& operator<<(std::ostream& os, const String& str);	
	friend std::istream& operator>>(std::istream& is, String& str);
	friend bool operator==(const String& first, const String& second);
	friend bool operator!=(const String& first, const String& second);
};

bool operator==(const String& first, const String& second);
bool operator!=(const String& first, const String& second);
