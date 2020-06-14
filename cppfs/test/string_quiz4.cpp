#include <cstring>
#include <iostream>

class String
{
public:
	
	String(const String& other)
	{
		m_str = Strdup(other.m_str);
		std::cout << "CCtor" << std::endl;
	}

	~String()
	{ 
		delete[] m_str;
	}
	String& operator=(const String& other);
	static String CreateInstance(const char* cstr);
	
	void Print()
	{
		std::cout << m_str << std::endl;
	}

private:
	String(const char* cstr)
		: m_str(Strdup(cstr)) 
	{ 
	}
	char* Strdup(const char* cstr = 0);
	char *m_str;
};

char *String::Strdup(const char* cstr)
{
	char *string = new char[strlen(cstr) + 1];
	strcpy(string, cstr);
	
	return string;
}

String& String::operator=(const String& other)
{
	char *temp = Strdup(other.m_str);
	delete[] m_str;
	m_str = temp;
	
	return *this;
}


String String::CreateInstance(const char* cstr)
{
	String s1(cstr);
	std::cout << "CreateInstance" << std::endl;
	return s1;
}

void GetAStringByValue(String s)
{
	 // just to see where the cctor is called, whether in the caller or
	 // in the callee
}


int main()
{
	
	String s1 = String::CreateInstance("Hello");
	String s2 = s1;
	//GetAStringByValue(s1);
	//String::CreateInstance("Hello");
	//String s2 = String::CreateInstance("Yoav");
	// String s2; // not allowed 
	// String s3("Not allowed");
	
	//s1.Print();
	/*
	s2.Print();
	String s3(s1);
	s3.Print();
	s2 = s1;
	s2.Print();
	*/
	
	return 0;
}







