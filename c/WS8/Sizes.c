/*****************************
*	Sizes of vars and types
*	Author: Yoav Klein
*	Reviewer: Alex Zilberman
*	Date: 23.5.19
******************************* */

#include <string.h>
#include <stdio.h>

#define SIZEOFVAR(x) ((long)(&x + 1) - (long)(&x))
#define SIZEOFTYPE(x) ((int)(long)((x*)0 + 1))

int main()
{
	char *s;
 	gets(s);
 	
 	return 1;
}
