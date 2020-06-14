#include <iostream>
#include <stdio.h>
#include <string.h>

int main()
{
	char buffer[40];
	
	int delim = sprintf(buffer, "%d;%d;%c", 4, 200, 0);
	
	const char *string = "YoavKleinKarmiel";
	std::copy(string, string + strlen(string), buffer + delim);
	
	for(int i = 0; i < 40; ++i)
	{
		if(buffer[i] == '\0')
		{
			std::cout << '*';
		}
		else
		{
			std::cout << buffer[i];
		}
	}

	return 0;
}
