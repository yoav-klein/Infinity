#include <cstring>
#include <cstdio>

#include "string.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }

#define CHECK(x, y) if((x) != (y)) { flag = 0; }


int TestString()
{
	int flag = 1;
	
	// test initialization
	String s1("Hello");
	const String s2(s1);
	String s3 = "Glida";
	
	CHECK(0, strcmp("Hello", s1.ToCStr()));
	CHECK(0, strcmp(s2.ToCStr(), s1.ToCStr()));
	
	// check Length
	CHECK(strlen("Hello"), s1.Length());
	
	//check assignment of another String
	s1 = s3;
	CHECK(0, strcmp("Glida", s1.ToCStr()))
	
	//check assignment of string literal
	s1 = "Burekas";
	CHECK(0, strcmp("Burekas", s1.ToCStr()))
	
	// check s1 == s2
	CHECK(0, s1 == s3)
	CHECK(1, s1 == s1);
	
	//check s1 != s2
	CHECK(1, s1 != s3)
	CHECK(0, s3 != s3)
	
	// check s1 == "GG"
	CHECK(1, "Burekas" == s1)
	CHECK(0, "urekas" == s1)
	CHECK(1, s1 == "Burekas")
	CHECK(0, s1 == "urekas")
	
	// check < >
	CHECK(1, s1 < s3)
	CHECK(0, s1 > s3)
	
	// check []
	s1[3] = 'A';
	CHECK(0, strcmp("BurAkas", s1.ToCStr()))
	CHECK(1, 'B' == s1[0])
	
	// check concat
	s1.Concat(s3);
	CHECK(0, strcmp("BurAkasGlida", s1.ToCStr()))
	CHECK(12, s1.Length())
	
	return flag;
}		

int TestCin()
{
	int flag = 1;
	
	String s;
	std::cin >> s; //Enter Jerusalem
	
	std::cout << s;
	
	CHECK(0, strcmp("Jerusalem", s.ToCStr()))
	CHECK(9, strlen(s.ToCStr()));
	CHECK(9, s.Length());

	return flag;
}


int main()
{
	RUN_TEST(TestString);
	RUN_TEST(TestCin);
	
	return 0;
}
