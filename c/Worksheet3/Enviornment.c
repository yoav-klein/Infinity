/***********************************************************************
*																		*
*                               Enviornment								*
*																		*
*								Author: Yoav Klein						*
*																		*
************************************************************************/


#include <stdio.h> /* printf */
#include <string.h> /*strlen */
#include <stdlib.h> /* malloc */
#include <ctype.h>	/* tolower */
#define EOS '\0'

void PrintEnv(char **new_env)
{
	while(*new_env)
	{
		printf("%s\n", *new_env);
		++new_env; 
	} 
}

void LCCopy(char *dest, const char *src)
{
	while(*src != EOS)
	{
		*dest = tolower(*src);
		src++;
		dest++;
	} 
	
	*dest = EOS;
}

void FreePointers(char **new_env)
{
	char **new_env_runner = new_env;
	
	while(NULL != *new_env_runner)
	{
		free(*new_env_runner);
		++new_env_runner;
	}
	
	free(new_env);
}

int CreateBuffer(char **old_env, char **new_env)
{
	int current_size = 0;
	char **old_env_runner = old_env;
	char **new_env_runner = new_env;
	
	while(NULL != *old_env_runner)
	{
		current_size = strlen(*old_env_runner);
		*new_env_runner = (char *)malloc(sizeof(char) * current_size + 1);
		
		if(*new_env_runner == NULL)
		{
			FreePointers(new_env);
			return (0);
		}
		
		LCCopy(*new_env_runner, *old_env_runner);
		new_env_runner++;
		old_env_runner++;
	}
	
	new_env_runner = NULL;
}

int main(int argc, char **argv, char **env)
{	
	int did_allocation_succeed = 0;
	int count = 0;
	char **env_runner = env;
	char **new_env = NULL;
	
	while(NULL != *env_runner)
	{
		++count;
		++env_runner;
	}
	new_env = malloc(sizeof(char *) * count + 1);
	
	if(NULL != new_env)
	{
		did_allocation_succeed = CreateBuffer(env, new_env);
		if(1 == did_allocation_succeed)
		{
			PrintEnv(new_env);
		}
		else
		{
			printf("Allocation failed\n");
		}
	}
	
	FreePointers(new_env);
	
	return 0;

}
