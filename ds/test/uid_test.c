#include <stdio.h>

#include "../include/uid.h"


#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

int Test()
{
	int flag = 1;
	
	unid_t uid = UIDCreate();
	unid_t uid2 = UIDCreate();
	
	if(uid.counter != uid2.counter - 1)
	{
		flag = 0;
	}
	
	if(uid.time.tv_sec != uid2.time.tv_sec)
	{
		flag = 0;
	}
	
	if(uid.pid != uid2.pid)
	{
		flag = 0;
	}
	
	if(!UIDIsSame(uid, uid))
	{
		flag = 0;
	}	
	
	if(UIDIsSame(uid, uid2))
	{
		flag = 0;
	}
	
	if(UIDIsBad(uid))
	{
		flag = 0;
	}
	
	if(!UIDIsBad(bad_uid))
	{
		flag = 0;
	}
	
	return flag;
	
} 

int main()
{
	
	RUN_TEST(Test);
	
	return 1;

}
