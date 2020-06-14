
#include <iostream>
#include <cstdio>
#include <cstring>

#include "rcstring.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }

using namespace ilrd;

int BasicStringTest()
{
	int flag = 1;	
	
	// test creation
	RCString s1("Jerusalem");
	CHECK(0, strcmp(s1.ToCStr(), "Jerusalem"))
	
	// test copying;
	RCString s2(s1);
	CHECK(0, strcmp(s2.ToCStr(), "Jerusalem"));
	
	// test operator []
	s1[0] = 'j';
	CHECK(0, strcmp(s1.ToCStr(), "jerusalem"));
	
	// test operator [] const version
	const RCString s3("Israel");
	
	// s3[0] = 't'; // won't compile
	CHECK('I', s3[0]);
	
	// test assignment operator
	s1 = s3;
	CHECK(0, strcmp(s3.ToCStr(), s1.ToCStr()));
	
	// test length
	CHECK(6, s3.Length());
	
	// test IsShared
	CHECK(true, s3.IsShared());
	CHECK(false, s2.IsShared());

	// test operator <<
	std::cout << s2 << " " << s1 << " " << s3 << std::endl;
	
	// test < > == !=
	CHECK(true, (s1 == s3));
	CHECK(false, (s1 == s2));
	CHECK(true, (s1 != s2));
	CHECK(false, (s1 != s3));
	CHECK(true, (s1 < s2));
	CHECK(false, (s1 > s2));
	
		
	return flag;
}		

int IstreamTest()
{
	int flag = 1;
	
	RCString s1("Maradona");
	std::cin >> s1; // insert Pupik
	
	CHECK(0, strcmp("Pupik", s1.ToCStr()));
	
	return flag;
}

int BracketsTest()
{
	int flag = 1;

	RCString s1("Yoav");
	RCString s2(s1);
	
	s2[0] = 'y';
	
	CHECK(s1[0], 'Y');
	CHECK(s2[0], 'y');

	return flag;
}

int main()
{
	RUN_TEST(BasicStringTest);
	RUN_TEST(IstreamTest);
	RUN_TEST(BracketsTest);
	
	return 0;
}
