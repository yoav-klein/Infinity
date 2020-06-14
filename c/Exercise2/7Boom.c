
#include <stdio.h>

int IfSevenIncluded(int num)
{
	while(0 != num)
	{
		if(7 == num % 10)
		{
			return 0;
		}
		
	return 1;	
	}
}

void SevenBoom(int start, int end)
{
	
	int i;
	
	for(i = start; i <= end; i++)
	{
		if(0 == i % 7 || 0 == IfSevenIncluded(i))
		{
			printf("BOOM\n");
		}
		else
		{
			printf("%d\n", i);
		}
	}

}

int main()
{
	int start=2, end=30;
	SevenBoom(start, end);
		
	
}
