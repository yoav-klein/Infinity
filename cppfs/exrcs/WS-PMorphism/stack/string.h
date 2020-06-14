#include <cstddef> // size_t
#include <iostream>

class String
{
	friend std::ostream& operator<<(std::ostream& os, const String& str);	
	friend std::istream& operator>>(std::istream& is, String& str);
	
public:
	String(const char *str);
	String(const String& s1);
	~String();
	
	String& operator=(const String& other);
	String& operator=(const char *str_literal);
	bool operator==(const String& other) const;
	bool operator!=(const String& other) const;
	bool operator==(const char* str_literal) const;
	bool operator>(const String& other) const;
	bool operator<(const String& other) const;
	char& operator[](size_t index) const;
	
	char* ToCStr() const;
	String& Concat(const String& other);
	size_t Length() const;
	
private:
	char *string;
	void Construct(const char* str_literal);
};

bool operator==(const char *str_literal, const String& str);
