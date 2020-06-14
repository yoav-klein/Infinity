/**********************************************
*	Calculator Utilities
*	Author: Yoav Klein
*	Reviewer: Sasha baron-cohen
*	Date: 7.7.19
************************************************/

#include <stdlib.h> /* malloc */
#include <math.h> /* pow */

#include "../include/stack.h" /* STACKCreate */

#define STACK_SIZE 50
#define ASCII_SIZE 256

enum status {OK, E_MATH, E_SYNTAX, E_MALLOC};
enum precedence { PRECEDENCE, IS_RIGHT };

typedef struct calc_struct 
{
	stack_t *num_stack;
	stack_t *op_stack;
	char (*prec_table)[2];
} calc_utils_t;


void CUDestroy(calc_utils_t *cu);

double CUGetResult(calc_utils_t *cu);

double CUGetResult(calc_utils_t *cu);

enum status OpHandler(char **str, calc_utils_t *cu);

enum status NumHandler(char **str, calc_utils_t *cu);

enum status ErrorHandler(char **str, calc_utils_t *cu);

enum status SpaceHandler(char **str, calc_utils_t *cu);

enum status EOSHandler(char **str, calc_utils_t *cu);

static void PrecedenceTableInit(char (*prec_table)[2]);

static enum status LastTwoNumsCalc(calc_utils_t *cu);

calc_utils_t *CUCreate()
{	
	char (*prec_table)[2];	
	calc_utils_t *cu = (calc_utils_t*)malloc(sizeof(calc_utils_t));
	if(NULL == cu)
	{
		return NULL;
	}
	
	cu->num_stack = STACKCreate(STACK_SIZE + 1, sizeof(double));
	if(NULL == cu->num_stack)
	{
		free(cu);
		return NULL;
	}
	
	cu->op_stack = STACKCreate(STACK_SIZE, sizeof(char));
	if(NULL == cu->op_stack)
	{
		STACKDestroy(cu->num_stack);
		free(cu);
		return NULL;
	}
	
	prec_table = calloc(ASCII_SIZE * 2, sizeof(char));
	if(NULL == prec_table)
	{
		CUDestroy(cu);
		return NULL;
	} 
	
	cu->prec_table = prec_table;
	PrecedenceTableInit(prec_table);
	
	return cu;
}

void CUDestroy(calc_utils_t *cu)
{
	free(cu->prec_table);
	STACKDestroy(cu->num_stack);
	STACKDestroy(cu->op_stack);
	free(cu);
}

double CUGetResult(calc_utils_t *cu)
{
	double result = *(double*)STACKPeek(cu->num_stack);
	
	return result;
}

enum status OpHandler(char **str, calc_utils_t *cu)
{
	char (*prec_t)[2] = cu->prec_table;
	enum status status = OK;
	int last_op = 0;
	int curr_op = **str;
	
	++*str;
	
 	/* if op_stack is empty, just push this one */
	if(0 == STACKSize(cu->op_stack) || '(' == curr_op)
	{
		return STACKPush(cu->op_stack, &curr_op) == 0 ? OK : E_MALLOC;	
	}
	
	last_op = *(char*)STACKPeek(cu->op_stack);
	
	/* first condition: status is ok
	second: op_stack isn't empty.
	third: if precedence of the last operator is higher, we want
	to calculate it, unless it's right associative (^) in that case we 
	don't want to calculate yet. 
	*/
	
	while(OK == status && 0 < STACKSize(cu->op_stack) && 
	(prec_t[curr_op][PRECEDENCE] < prec_t[last_op][PRECEDENCE] || 
	(prec_t[curr_op][PRECEDENCE] == prec_t[last_op][PRECEDENCE] 
	&& !prec_t[last_op][IS_RIGHT])))
	{
		status = LastTwoNumsCalc(cu);
		if(STACKSize(cu->op_stack) > 0)
		{
			last_op = *(char*)STACKPeek(cu->op_stack);
		}
	}
	
	if(OK != status)
	{
		return status;
	}
	
	return STACKPush(cu->op_stack, &curr_op) == 0 ? OK : E_MALLOC;
}

enum status ParanthesisHandler(char **str, calc_utils_t *cu)
{
	enum status status = OK;
	++*str;
	
	while(STACKSize(cu->op_stack) > 0 && OK == status 
	&& '(' != *(char*)STACKPeek(cu->op_stack))
	{
		status = LastTwoNumsCalc(cu);	
	}
	
	if(0 == STACKSize(cu->op_stack)) 
	{
		return E_SYNTAX;
	}
	
	STACKPop(cu->op_stack);
	
	return status;
}

enum status NumHandler(char **str, calc_utils_t *cu)
{
	double num = strtod(*str, str);
	
	return 0 == STACKPush(cu->num_stack, &num) ? OK : E_MALLOC;
}

enum status ErrorHandler(char **str, calc_utils_t *cu)
{
	double fake_result = 0;
	(void)str;
	(void)cu;
	
	/* in case the stack is empty, avoiding segfault on STACKPeek */
	STACKPush(cu->num_stack, &fake_result);
	return E_SYNTAX;
}

enum status SpaceHandler(char **str, calc_utils_t *cu)
{
	(void)cu;
	(void)str;
	
	++*str;
	
	return OK;
}

enum status EOSHandler(char **str, calc_utils_t *cu)
{
	enum status status =  OK;

	(void)str;
	
	while(1 < STACKSize(cu->num_stack) && OK == status)
	{
		status = LastTwoNumsCalc(cu);
	}
	
	/* check if there aren't '(' on the op_stack */
	if(STACKSize(cu->op_stack) > 0)
	{
		status = E_SYNTAX;
	}
	return status;
}

static enum status LastTwoNumsCalc(calc_utils_t *cu)
{
	double first = 0, second = 0;
	double result = 0;	
	
	char operator = *(char*)STACKPeek(cu->op_stack);
	
	second = *(double*)STACKPeek(cu->num_stack);
	STACKPop(cu->num_stack);
	
	first = *(double*)STACKPeek(cu->num_stack);
	STACKPop(cu->num_stack);
		
	STACKPop(cu->op_stack);
	
	switch(operator)
	{
		case '+': 
			result = first + second;
			break;
		
		case '-':
			result = first - second;
			break;
		
		case '*':
			result = first * second;
			break;
		
		case '^':
			result = pow(first, second);
			break;
			
		case '/':
			if(second != 0)
			{
				result = first / second;
				break;
			}
		
			STACKPush(cu->num_stack, &result);
			return E_MATH;
	}
	
	return STACKPush(cu->num_stack, &result) == 0 ? OK : E_MALLOC;		
}

static void PrecedenceTableInit(char (*prec_table)[2])
{
	prec_table[(int)'+'][PRECEDENCE] = 1;	
	prec_table[(int)'+'][IS_RIGHT] = 0; 
	
	prec_table[(int)'-'][PRECEDENCE] = 1;	
	prec_table[(int)'-'][IS_RIGHT] = 0;
	
	prec_table[(int)'*'][PRECEDENCE] = 2;	
	prec_table[(int)'*'][IS_RIGHT] = 0;
	
	prec_table[(int)'/'][PRECEDENCE] = 2;	
	prec_table[(int)'/'][IS_RIGHT] = 0;
	
	prec_table[(int)'^'][PRECEDENCE] = 3;	
	prec_table[(int)'^'][IS_RIGHT] = 1;
	
	prec_table[(int)'('][PRECEDENCE] = 0;
	prec_table[(int)'('][IS_RIGHT] = 0;
	
}

