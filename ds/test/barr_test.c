#include <stdio.h>
#include <stdlib.h>

#include "../include/barr.h"


int BARRSetBit_Test()
{
	int flag = 1;
	
	size_t num = 32;
	num = BARRSetBit(num, 0, 1);
	printf("First test: Expected result: 33, Result: %lu\n", num);
	if(num != 33)
	{
		flag = 0;
	}
	
	num = 1;
	num = BARRSetBit(num, 5, 1);
	printf("Second test: Expected result: 33, Result: %lu\n", num);
	if(num != 33)
	{
		flag = 0;
	}
	
	num = 5;
	num = BARRSetBit(num, 2, 0);
	printf("Third test: Expected result: 1, Result: %lu\n", num);
	if(num != 1)
	{
		flag = 0;
	}
	
	return flag;
}

int BARRSetOn_Test()
{
	int flag = 1;
	size_t num = 0;
	
	num = BARRSetOn(num, 0);
	printf("First Test: Expected result: 1, result: %lu\n", num);
	if(num != 1)
	{
		flag = 0;
	} 
	
	num = 7;
	num = BARRSetOn(num, 3);
	printf("First Test: Expected result: 15, result: %lu\n", num);
	if(num != 15)
	{
		flag = 0;
	}
	
	num = 254;
	num = BARRSetOn(num, 0);
	printf("Third Test: Expected result: 255, result: %lu\n", num);
	if(num != 255)
	{
		flag = 0;
	}
	
	return flag; 
}

int BARRSetOff_Test()
{
	int flag = 1;
	size_t num = 7;
	
	num = BARRSetOff(num, 0);
	printf("First Test: Expected result: 6, result: %lu\n", num);
	if(num != 6)
	{
		flag = 0;
	} 
	
	num = 255;
	num = BARRSetOff(num, 7);
	printf("First Test: Expected result: 127, result: %lu\n", num);
	if(num != 127)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRSetOff(num, 1);
	printf("Third Test: Expected result: 13, result: %lu\n", num);
	if(num != 13)
	{
		flag = 0;
	}
	
	return flag; 
}

int BARRFlip_Test()
{
	int flag = 1;
	size_t num = 7;
	
	num = BARRFlip(num, 0);
	printf("First Test: Expected result: 6, result: %lu\n", num);
	if(num != 6)
	{
		flag = 0;
	} 
	
	num = 254;
	num = BARRFlip(num, 0);
	printf("First Test: Expected result: 255, result: %lu\n", num);
	if(num != 255)
	{
		flag = 0;
	}
	
	num = 13;
	num = BARRFlip(num, 1);
	printf("Third Test: Expected result: 15, result: %lu\n", num);
	if(num != 15)
	{
		flag = 0;
	}
	
	num = 7;
	num = BARRFlip(num, 2);
	printf("Fourth Test: Expected result: 3, result: %lu\n", num);
	if(num != 3)
	{
		flag = 0;
	}
	
	return flag; 
}


int BARRMirror_Test()
{
	int flag = 1;
	size_t num = 1;	

	num = BARRMirror(num);
	printf("First Test: Expected result: 0x8000000000000000, result: %lx\n", num);
	if(num != 0x8000000000000000)
	{
		flag = 0;
	} 
	
	num = 0xffff000000000000;
	num = BARRMirror(num);
	printf("Second Test: Expected result: 0xffff, result: %lx\n", num);
	if(num != 0xffff)
	{
		flag = 0;
	} 
	
	num = 0xf0f0f0f000000000;
	num = BARRMirror(num);
	printf("Second Test: Expected result: 0xf0f0f0f, result: %lx\n", num);
	if(num != 0xf0f0f0f)
	{
		flag = 0;
	} 
	
	return flag; 	
}

int BARRMirrorLUT_Test()
{
	int flag = 1;
	size_t num = 1;	

	num = BARRMirrorLUT(num);
	printf("First Test: Expected result: 0x8000000000000000, result: %lx\n", num);
	if(num != 0x8000000000000000)
	{
		flag = 0;
	} 
	
	num = 0xffff000000000000;
	num = BARRMirrorLUT(num);
	printf("Second Test: Expected result: 0xffff, result: %lx\n", num);
	if(num != 0xffff)
	{
		flag = 0;
	} 
	
	num = 0xf0f0f0f000000000;
	num = BARRMirrorLUT(num);
	printf("Second Test: Expected result: 0xf0f0f0f, result: %lx\n", num);
	if(num != 0xf0f0f0f)
	{
		flag = 0;
	} 
	
	return flag; 	
}

int BARRRotateLeft_Test()
{
	int flag = 1;
	size_t num = 7;
	
	num = BARRRotateLeft(num, 1);
	printf("First Test: Expected result: 14, result: %lu\n", num);
	if(num != 14)
	{
		flag = 0;
	} 
	
	num = 50;
	num = BARRRotateLeft(num, 2);
	printf("First Test: Expected result: 200, result: %lu\n", num);
	if(num != 200)
	{
		flag = 0;
	}
	
	num = 0x0f00000000000000;
	num = BARRRotateLeft(num, 8);
	printf("Third Test: Expected result: 15, result: %lu\n", num);
	if(num != 15)
	{
		flag = 0;
	}
	
	return flag; 	
}

int BARRRotateRight_Test()
{
	int flag = 1;
	size_t num = 8;
	
	num = BARRRotateRight(num, 1);
	printf("First Test: Expected result: 4, result: %lu\n", num);
	if(num != 4)
	{
		flag = 0;
	} 
	
	num = 50;
	num = BARRRotateRight(num, 1);
	printf("First Test: Expected result: 25, result: %lu\n", num);
	if(num != 25)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRRotateRight(num, 4);
	printf("Third Test: Expected result: 0xf000000000000000, result: %lx\n", num);
	if(num != 0xf000000000000000)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRRotateRight(num, 64);
	printf("Fourth Test: Expected result: 15, result: %lu\n", num);
	if(num != 15)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRRotateRight(num, 68);
	printf("Third Test: Expected result: 0xf000000000000000, result: %lx\n", num);
	if(num != 0xf000000000000000)
	{
		flag = 0;
	}
	
	return flag; 	
}

int BARRCountOn_Test()
{
	int flag = 1;
	size_t num = 8;
	
	num = BARRCountOn(num);
	printf("First Test: Expected result: 1, result: %lu\n", num);
	if(num != 1)
	{
		flag = 0;
	} 
	
	num = 50;
	num = BARRCountOn(num);
	printf("First Test: Expected result: 3, result: %lu\n", num);
	if(num != 3)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRCountOn(num);
	printf("Third Test: Expected result: 4, result: %lu\n", num);
	if(num != 4)
	{
		flag = 0;
	}
	
	num = 0xfff0ff00ffff0000;
	num = BARRCountOn(num);
	printf("Fourth Test: Expected result: 36, result: %lu\n", num);
	if(num != 36)
	{
		flag = 0;
	}
	
	return flag; 	

}

int BARRCountOnLUT_Test()
{
	int flag = 1;
	size_t num = 8;
	
	num = BARRCountOnLUT(num);
	printf("First Test: Expected result: 1, result: %lu\n", num);
	if(num != 1)
	{
		flag = 0;
	} 
	
	num = 50;
	num = BARRCountOnLUT(num);
	printf("First Test: Expected result: 3, result: %lu\n", num);
	if(num != 3)
	{
		flag = 0;
	}
	
	num = 15;
	num = BARRCountOnLUT(num);
	printf("Third Test: Expected result: 4, result: %lu\n", num);
	if(num != 4)
	{
		flag = 0;
	}
	
	num = 0xfff0ff00ffff0000;
	num = BARRCountOnLUT(num);
	printf("Fourth Test: Expected result: 36, result: %lu\n", num);
	if(num != 36)
	{
		flag = 0;
	}
	
	return flag; 	

}


int BARRCountOff_Test()
{
	int flag = 1;
	size_t num = 8;
	
	num = BARRCountOff(num);
	printf("First Test: Expected result: 63, result: %lu\n", num);
	if(num != 63)
	{
		flag = 0;
	} 
	
	num = 15;
	num = BARRCountOff(num);
	printf("First Test: Expected result: 60, result: %lu\n", num);
	if(num != 60)
	{
		flag = 0;
	}
	
	num = 0xff;
	num = BARRCountOff(num);
	printf("Third Test: Expected result: 56, result: %lu\n", num);
	if(num != 56)
	{
		flag = 0;
	}
	
	return flag; 	

}

int BARRIsOn_Test()
{
	int flag = 1;
	size_t num = 0xff0000ff000f000f;
	int res = 0; 
	
	res = BARRIsOn(num, 3);
	printf("First Test: Expected result: 1, result: %d\n", res);
	if(res != 1)
	{
		flag = 0;
	} 
	
	res = BARRIsOn(num, 7);
	printf("First Test: Expected result: 0, result: %d\n", res);
	if(res != 0)
	{
		flag = 0;
	} 
	
	res = BARRIsOn(num, 60);
	printf("First Test: Expected result: 1, result: %d\n", res);
	if(res != 1)
	{
		flag = 0;
	} 
	
	return flag; 	

}

int BARRIsOff_Test()
{
	int flag = 1;
	size_t num = 0xff0000ff000f000f;
	int res = 0; 
	
	res = BARRIsOff(num, 3);
	printf("First Test: Expected result: 0, result: %d\n", res);
	if(res != 0)
	{
		flag = 0;
	} 
	
	res = BARRIsOff(num, 7);
	printf("First Test: Expected result: 1, result: %d\n", res);
	if(res != 1)
	{
		flag = 0;
	} 
	
	res = BARRIsOff(num, 60);
	printf("First Test: Expected result: 0, result: %d\n", res);
	if(res != 0)
	{
		flag = 0;
	} 
	
	return flag; 	

}

int main()
{
	
	TEST(BARRSetBit_Test);
	TEST(BARRSetOn_Test);
	TEST(BARRSetOff_Test);
	TEST(BARRFlip_Test);
	TEST(BARRMirror_Test);
	TEST(BARRMirrorLUT_Test);
	TEST(BARRRotateLeft_Test);
	TEST(BARRRotateRight_Test);
	TEST(BARRCountOn_Test);
	TEST(BARRCountOff_Test);
	TEST(BARRCountOnLUT_Test);
	TEST(BARRIsOn_Test);
	TEST(BARRIsOff_Test);
	
	return 1;
}
