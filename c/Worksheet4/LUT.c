
/********************************************************************
*							Author: Yoav Klein						*
*							Date: 15.5.19							*
*							Reviwer: Eran Segal    					*
*********************************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /* exit */

void (*functions[256])();

void UsingIf()
{
	char c = 0;
	system("stty -icanon -echo");
	
	while (1)
	{
		c = getchar();
		
		if(65 == c)
		{
			printf("A pressed\n");
		}
		else if(84 == c)
		{
			printf("T pressed\n");
		}
		else if(27 == c)
		{
			system("stty icanon echo");
			exit(0);
		}
	}
}

void DoNothing()
{
	;
}

void PrintT()
{
	printf("T pressed\n");
}

void PrintA()
{
	printf("A pressed\n");
}

void Escape()
{
	exit(0);
}

void UsingSwitch()
{
	char c = 0;
	system("stty -icanon -echo");
	
	while (1)
	{
		c = getchar();
		switch(c)
		{
			case 65: 
				printf("A pressed\n");
				break;
			case 84: 
				printf("T pressed\n");
				break;
			case 27:
				system("stty icanon echo");
				exit(0);		
		}
	}
}

void UsingLUT()
{
	
	char c = 0;	
	int i =0;
	
	system("stty -icanon -echo");
	
	for(; i < 256; i++)
	{
		if(65 != i && 84 != i && 27 != i)
		{
			*(functions + i) = DoNothing;
		}
	}
	
	(*(functions + 65)) = PrintA;
	(*(functions + 84)) = PrintT;
	(*(functions + 27)) = Escape;
	
	while(1)
	{
		c = getchar();
		(*(functions + c))();
	}
	system("stty icanon echo");
	
}

int Multiple(int x)
{
	return x * 2;
}



int main()
{

	UsingLUT();
	
	return 0;
}
