#include <stdio.h>
#include "../include/calculator.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int CalculatorTest()
{
	double d = 0;
	enum status status = OK;
	int flag = 1;
	
	status = Calculate("5 + 2", &d);
	if(7 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2+1+10+20", &d);
	if(33 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2-2-3", &d);
	if(-3 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("400*2", &d);
	if(800 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2", &d);
	if(2 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("300*2-1", &d);
	if(599 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("300 + 2 *3", &d);
	if(306 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("300/0", &d);
	if(1 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(200 * 2)", &d);
	if(400 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(2+3)*1-3-(4)+2", &d);
	if(0 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(2+3)*(3-1)", &d);
	if(10 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("((((8-1)+3)*6)-((3+7)*2))", &d);
	if(40 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("4 + 2^2", &d);
	if(8 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2 ^ 3 ^ 2 + 5", &d);
	if(517 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2*(2^3)^2+5-20", &d);
	if(113 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2/4*(2^3)^2+5-20", &d);
	if(17 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(2 * 4", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2 * 4)", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(((5 + 2) * (3+4)*3))", &d);
	if(147 != d || 0 != status)
	{
		flag = 0;
	}	
	
	status = Calculate("40 + 2 / 2 - 3 * 2", &d);
	if(35 != d || 0 != status)
	{
		flag = 0;
	}	
	
	status = Calculate("40 + 2 / 2 - (3 * (2)) + 20 / 2^(2+2) - (10 / 2.5)", &d);
	if(32.25 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("2 + (3 * 5 + 2^2^1 +50 / 25 - 13 + ((23)))", &d);
	if(33 != d || 0 != status)
	{
		flag = 0;
	}
	
	status = Calculate("*2 + 2* 3 ++ 2", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	status = Calculate("*40 + 2 / 2 - 3 * 2", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	status = Calculate("40 + 2 / 2 - (3 * 2))", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	status = Calculate("(40 + 2 / 2 - (3 * (2)) + 20 / 2^2 - 10 / 2.5", &d);
	if(2 != status)
	{
		flag = 0;
	}
	
	return flag;

}

int main()
{
	RUN_TEST(CalculatorTest);		
	
	return 0;
}
