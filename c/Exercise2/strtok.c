#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EOS '\0'

char *Strtok(char *str, const char *delim);

void StrtokTry()
{
	char str[300] = "http://www.g///////oogle.com//////";
	char *delim = ":./";
	char *ptr = NULL;
	
	ptr = Strtok(str, delim);
	printf("%s\n", ptr);
	
	ptr = Strtok(NULL, delim);
	printf("%s\n", ptr);
	
	ptr = Strtok(NULL, delim);
	printf("%s\n", ptr);
	
	ptr = Strtok(NULL, delim);
	printf("%s\n", ptr);
	
	ptr = Strtok(NULL, delim);
	printf("%s\n", ptr);
	
	
}

char *Strtok(char *str, const char *delim)
{
	static char *next_ptr = NULL;
	char *ret_ptr = NULL;
	char *runner = NULL;
	int where_next_delim = 0;
	int num_of_delims = 0;
	
	if(str != NULL)
	{
		ret_ptr = str;
	}
	else
	{
		ret_ptr = next_ptr;
		if(EOS == *next_ptr)
		{
			return NULL;
		}
	}
	
	runner = ret_ptr;
	
	where_next_delim = strcspn(runner, delim);
	runner = ret_ptr + where_next_delim;
	
	/* delete delims with EOS */
	num_of_delims = strspn(runner, delim);
	while(num_of_delims > 0)
	{
		*runner = EOS;
		num_of_delims--;
		runner++;
	}
	next_ptr = runner;
		
	
	return (ret_ptr);
}


int main()
{
	StrtokTry();
	
	return 0;

}
