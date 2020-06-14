/*************************************************************************
*					Author: Yoav Klein									 *
*					Date: 16.5.19										 *
*					Reviewer: Mishel Liberman 							 * 
*																		 * 
**************************************************************************/


#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include <stdlib.h> /* malloc */
#include <errno.h> /* errno */

#define NEW_LINE 10
#define UNUSED(x) (void)(x);
#define STAGES_NUM 5
#define INPUT_SIZE 50

enum status {Failed, Succeed};


static char *file_name;

int CompareStrings(char *input, char *stage);
int DontCompare(char *input, char *stage);
enum status AppendOperate(char *str);
enum status CountOperate(char *str);
enum status ExitOperate(char *str);
enum status RemoveOperate(char *str);
enum status PrependOperate(char *str);

struct Stage /* The type of each link in the chain of responsibilit */
{
	char *command;
	int (*compare)(char *input, char *stage_str);
	enum status (*operate)(char*);
};



int CompareStrings(char *input, char *stage_str)
{
	int cmp = 0;
	
	cmp = strcmp(input, stage_str);

	if(0 == cmp)
	{
		return 1;
	}
	else if('<' == *stage_str && '<' == *input)  /* if we're in the stage */
	{											 /* of prepending, check */
		return 1;								 /*  the input's first char */
	}
	else
	{
		return 0;
	}
	
}

int DontCompare(char *input, char *stage) /* default compare function */   
{											/* returns 1 anyway */
	UNUSED(input);
	UNUSED(stage);
	
	return 1;
}


enum status AppendOperate(char *str)
{
	int status = 0;
	FILE* file = NULL;
	
	file = fopen(file_name, "a");
	if(NULL == file)
	{
		return Failed;
	}
	
	status = fputs(str, file);
	
	fclose(file);
	
	if(status > 0)
	{
		return Succeed;
	}
	else
	{
		return Failed;
	}
}

enum status RemoveOperate(char *str)
{
	int status = 0;
	UNUSED(str);
	
	status = remove(file_name);
	if(status == -1)
	{
		return Failed;
	}
	else
	{	
		printf("File removed\n");
		return Succeed;
	}
}

enum status CountOperate(char *str)
{
	int status = 0;
	int count = 0;
	int current = 0;
	FILE *file = NULL;
	
	UNUSED(str);
	
	file = fopen(file_name, "a+");
	if(NULL == file)
	{
		return Failed;
	}
	
	while(EOF != current)
	{	
		current = fgetc(file);
		if(NEW_LINE == current)
		{
			++count;
		}
	}
	
	status = fprintf(file, "%d\n", count);
	fclose(file);
	
	if(status > 0)
	{
		return Succeed;
	}
	else
	{
		return Failed;
	}
}

enum status ExitOperate(char *str)
{
	free(str);
	exit(0);
}

enum status PrependOperate(char *str)
{	
	int status = 0;
	FILE *temp = NULL;
	FILE *file = NULL;
	
	char c = 0;
	
	UNUSED (str);
	temp = fopen("TemporaryFile", "w+");
	file = fopen(file_name, "r");
	
	if(NULL == temp || NULL == file)
	{
		return Failed;
	}
	
	c = fgetc(file);
	while(c != EOF)    /* copying all the text in file to temp */
	{
		fputc(c, temp);
		c = fgetc(file);
	}
	
	fclose(file);
	file = fopen(file_name, "w+"); /* opening again to truncate */
	rewind(temp);  
	
	status = fputs((str + 1), file); /* copying from second char in the string */
	
	if(Failed == status)
	{
		return Failed;
	}
	
	c = fgetc(temp); 
	while(c != EOF)  /* copying the contents of temp */
	{
		status = fputc(c, file);
		if(EOF == status)
		{
			return Failed;
		}
		c = fgetc(temp);
	}
	
	fclose(file);
	remove("TemporaryFile");
	
	return Succeed;
}

void Operation()
{
	struct Stage stages[STAGES_NUM] = 
	{ 		{ "-exit\n", CompareStrings, ExitOperate },
			{ "-remove\n", CompareStrings, RemoveOperate },
		    { "-count\n", CompareStrings, CountOperate },
   		    { "<", CompareStrings, PrependOperate },
    	    { ".", DontCompare, AppendOperate },
	}; 	
	
	int i = 0;
	int compare = 0;
	int status = 0;
	
	char *input_str = (char *)malloc(INPUT_SIZE);
	if(NULL == input_str)
	{
		fprintf(stderr ,"Failed to allocate memory\n");
		exit(0);
	}
	
	while(1)
	{
		printf("Enter a string\n");
		i = 0;
		fgets(input_str, INPUT_SIZE, stdin);
		
		while(i < STAGES_NUM)
		{
			
			compare = stages[i].compare(input_str, stages[i].command);
			
			if(1 == compare)
			{
				status = stages[i].operate(input_str);
				if(0 == status)
				{
					fprintf(stderr, "%s\n", strerror(errno));
					free(input_str);
					exit(0);
				}
				break;
			}
			++i;
		}
	}
	
	free(input_str);
}



int main(int argc, char **argv)
{
	
	if(argc > 1)
	{
		file_name = *(argv + 1);
		printf("%s\n", file_name);
		Operation();
	}	
	else
	{
		printf("Enter file name, man!\n");
	}
	
	return 0;

}
