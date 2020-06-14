/**********************************************
*	Calculator
*	Author: Yoav Klein
*	Reviewer: Sasha baron-cohen
*	Date: 7.7.19
************************************************/

#include <stdlib.h> /* malloc */

#include "../include/calculator.h" /* Calculate */

#define ASCII_SIZE 256

enum state { WN, WO, ERROR, FINISH };

typedef struct calc_struct calc_utils_t;
typedef enum status (*handler_t)(char **str, calc_utils_t *cu);

struct transition
{
	enum state to;
	handler_t handler;
};

static void TransitionTableInit(struct transition(*trans_table)[2]);

enum status OpHandler(char **str, calc_utils_t *cu);

enum status NumHandler(char **str, calc_utils_t *cu);

enum status ErrorHandler(char **str, calc_utils_t *cu);

enum status SpaceHandler(char **str, calc_utils_t *cu);

enum status EOSHandler(char **str, calc_utils_t *cu);

enum status ParanthesisHandler(char **str, calc_utils_t *cu);

calc_utils_t *CUCreate();

double CUGetResult(calc_utils_t *cu);

void CUDestroy(calc_utils_t *cu);

enum status Calculate(const char *str, double *result)
{	
	char *my_str = (char*)str;
	struct transition transition;
	enum status new_status = OK;
	enum state fsm_state = WN;
	struct transition (*trans_table)[2];
		
	calc_utils_t *cu = CUCreate();
	if(NULL == cu)
	{
		return E_MALLOC;
	}
	
	trans_table = malloc(sizeof(struct transition) * ASCII_SIZE * 2);
	if(NULL == trans_table)
	{
		CUDestroy(cu);
		return E_MALLOC;
	}
	
	(void)result;
	TransitionTableInit(trans_table);
	
	while(OK == new_status && FINISH != fsm_state)
	{
		transition = trans_table[(int)*my_str][fsm_state];
		new_status = transition.handler(&my_str, cu);
		fsm_state = transition.to;
	}
	
	*result = CUGetResult(cu);
	
	CUDestroy(cu);
	free(trans_table);	
	
	return new_status;
}

static void TransitionTableInit(struct transition(*trans_table)[2])
{
	size_t i = 0, j = 0;

	for(i = 0; i < ASCII_SIZE; ++i)
	{
		for(j = 0; j < 2; ++j)
		{
			trans_table[i][j].to = ERROR;
			trans_table[i][j].handler = ErrorHandler;
		}
	}
	
	for(i = '0'; i < '9'; ++i)
	{
		trans_table[i][WN].to = WO;
		trans_table[i][WN].handler = NumHandler;
		trans_table[i][WO].to = ERROR;
		trans_table[i][WO].handler = ErrorHandler;
	}
	
	trans_table[(int)'+'][WO].to = WN;
	trans_table[(int)'+'][WO].handler = OpHandler;  
	trans_table[(int)'+'][WN].to = WO;
	trans_table[(int)'+'][WN].handler = NumHandler;
	
	trans_table[(int)'-'][WO].to = WN;
	trans_table[(int)'-'][WO].handler = OpHandler;  
	trans_table[(int)'-'][WN].to = WO;
	trans_table[(int)'-'][WN].handler = NumHandler;
	
	trans_table[(int)'*'][WO].to = WN;
	trans_table[(int)'*'][WO].handler = OpHandler;  
	trans_table[(int)'*'][WN].to = ERROR;
	trans_table[(int)'*'][WN].handler = ErrorHandler;

	trans_table[(int)'/'][WO].to = WN;
	trans_table[(int)'/'][WO].handler = OpHandler;  
	trans_table[(int)'/'][WN].to = ERROR;
	trans_table[(int)'/'][WN].handler = ErrorHandler;
	
	trans_table[(int)'^'][WO].to = WN;
	trans_table[(int)'^'][WO].handler = OpHandler;  
	trans_table[(int)'^'][WN].to = ERROR;
	trans_table[(int)'^'][WN].handler = ErrorHandler;
	
	trans_table[(int)' '][WO].to = WO;
	trans_table[(int)' '][WO].handler = SpaceHandler;
	trans_table[(int)' '][WN].to = WN;
	trans_table[(int)' '][WN].handler = SpaceHandler;
	
	trans_table[(int)'('][WO].to = ERROR;
	trans_table[(int)'('][WO].handler = ErrorHandler;
	trans_table[(int)'('][WN].to = WN;
	trans_table[(int)'('][WN].handler = OpHandler;

	trans_table[(int)')'][WO].to = WO;
	trans_table[(int)')'][WO].handler = ParanthesisHandler;
	trans_table[(int)')'][WN].to = ERROR;
	trans_table[(int)')'][WN].handler = ErrorHandler;

	trans_table[0][WO].to = FINISH;
	trans_table[0][WO].handler = EOSHandler;
	trans_table[0][WN].to = ERROR;
	trans_table[0][WN].handler = ErrorHandler;
}

