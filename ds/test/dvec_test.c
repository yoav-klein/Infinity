#include <stdio.h>
#include "../include/dvec.h"

#define RUN_TEST(x) if(1 == x()) { printf(#x " Success!\n"); } else { printf("Failed!\n"); } 

int DVECTest_1()
{
	int flag = 1;
	int test_var1 = 5;
	int test_var2 = 10;
	int res = 0;
	int *address = NULL;
	
	/* test creating, size and capacity */
	dvec_t* dvec = DVECCreate(4, 4);
	(void)res;
	
	printf("DVEC created!\n"); 
	
	if(4 != *((size_t*)dvec + 3) && 4 != DVECCapacity(dvec))
	{
		flag = 0;
	}
	
	if(0 != *((size_t*)dvec + 2) && 0 != DVECSize(dvec))
	{
		flag = 0;
	}
	
	if(4 != *((size_t*)dvec + 1))
	{
		flag = 0;
	}
	
	/* test pushing and size functions */
	res = DVECPushBack(dvec, &test_var1);   /* 1st element */
	if(1 != *((size_t*)dvec + 2) && 1 != DVECSize(dvec))
	{
		flag = 0;
	}
	
	res = DVECPushBack(dvec, &test_var2); /* 2nd element*/
	if(2 != *((size_t*)dvec + 2))
	{
		flag = 0;
	}
	
	if(test_var1 != *(*(int**)dvec))
	{
		flag = 0;
	}
	
	if(test_var2 != *(*(int**)dvec + 1))
	{
		flag = 0;
	}
	
	/* test getting item */
	address = DVECGetItemAddress(dvec, 0);
	if(*(int*)address != test_var1)
	{
		flag = 0;
	}
	
	address = DVECGetItemAddress(dvec, 1);
	if(*(int*)address != test_var2)
	{
		flag = 0;
	}
	
	
	
	/* test expanding of the array */
	if(4 != *((size_t*)dvec + 3))
	{
		flag = 0;
	}
	
	res = DVECPushBack(dvec, &test_var1); /* 3rd element */
	res = DVECPushBack(dvec, &test_var2); /* 4th element */
	
	if(8 != *((size_t*)dvec + 3) && 8 != DVECCapacity(dvec)) 
	{
		flag = 0;
	}
	
	
	/* test popping and shrinking */
	
	
	DVECPopBack(dvec); /* 3 remaining */
	if(3 != *((size_t*)dvec + 2) && 3!= DVECSize(dvec))
	{
		flag = 0;
	}
	
	DVECPopBack(dvec); /* 2 remaining */
	if(4 != *((size_t*)dvec + 3) && 4 != DVECCapacity(dvec))
	{
		flag = 0;
	}
		
	/* test reserve */
	res = DVECReserve(dvec, 15);
	if(15 != DVECCapacity(dvec))
	{
		flag = 0;
	} 
	
	DVECPushBack(dvec, &test_var2); /* 3 elemenets */
	DVECPushBack(dvec, &test_var2); /* 4 elements */
	
	res = DVECReserve(dvec, 2);
	if(5 != DVECCapacity(dvec))
	{
		flag = 0;
	} 
	
	DVECPushBack(dvec, &test_var1);
	if(10 != DVECCapacity(dvec))
	{
		flag = 0;
	}
	
	/* shrinking to zero */ 
	DVECPopBack(dvec); /* 4 elements */
	DVECPopBack(dvec); /* 3 elements */
	DVECPopBack(dvec); /* 2 elements */
	
	if(5 != DVECCapacity(dvec) && 2 != DVECSize(dvec))
	{
		flag = 0;
	}
	
	DVECPopBack(dvec); /* 1 element */
	if(2 != DVECCapacity(dvec))
	{
		flag = 0;
	}
	
	DVECPopBack(dvec); /* 0 */
	if(1 != DVECCapacity(dvec) && 0 != DVECSize(dvec))
	{
		flag = 0;
	}
	
	
	
	DVECPopBack(dvec);
	DVECDestroy(dvec);
	
	return flag;	
}

int main()
{
	RUN_TEST(DVECTest_1);


	return 1;	
}
