#include <stdio.h>
#include <string.h>

int IsRotation(char *str1, char *str2)
{
	char *runner1 = str1;
	char *runner2 = str2;
	size_t count = 0;
	size_t i = 0;
	size_t new = 0;
	
	for(i = 0; i < strlen(str2); ++i)
	{
		count = 0;
		runner1 = str1;
		runner2 = str2 + i;
		while(*runner1 == *runner2 && count < strlen(str1))
		{
			++count;
			++runner1;
			runner2 = str2 + (i + count) % strlen(str2);
		}
		
		if(count == strlen(str1))
		{
			return 1;
		}
		
		
	} 
	
	return 0;
}


int main()
{
	char *str1 = "Yaoav";
	char *str2 = "avYo";
	
	printf("%d\n", IsRotation(str1, str2));
	
	return 0;

}
