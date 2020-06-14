/****************************************************
*		Knights Tours
*		Author: Yoav Klein
*		Reviewer: Inbar Namdar
*		Date: 24.7.19
*****************************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include <time.h> /* time */
#include <unistd.h> /* sleep */

#include "barr.h" /* BARRIsOn */
#include "sort.h" /* InsertionSort */ 
#include "moves.h"

#define THREE_MINS 3
#define WID 8
#define HEI 8

enum status
{
	OK = 0,
	M_FAIL,
	T_FAIL
};


void PrintArr(int *arr)
{
	int i = 0;
	for(i = 0; i < 64; ++i)
	{
		printf("%d -> ", arr[i]);
	}
	
	printf("\n");
}

int IsLegalMove(int from, int to)
{
	int i = 0;
	
	for(i = 0; i < 8; ++i)
	{
		if(moves[from][i] == to)
		{
			return 1;
		} 
	}
	return 0;
}

enum status KTRec(int *arr, size_t board, int index, time_t start_time)
{
	int i = 0;
	int prev = 0;
	enum status status;
	
	if(time(NULL) - start_time > THREE_MINS)
	{
		return T_FAIL;
	}
	
	prev = arr[index - 1];
	if(BARRIsOn(board, prev))
	{
		return M_FAIL;
	}
	
	if(WID * HEI == index)
	{
		return OK;
	}
	
	board = BARRSetOn(board, prev);
	
	while(-1 < moves[prev][i])
	{
		arr[index] = moves[prev][i];
		status = KTRec(arr, board, index + 1, start_time);
		if(OK == status)
		{
			return OK;
		}
		++i;
	}
	
	return M_FAIL;
}

enum status KT(int *arr, int first)
{
	size_t board = 0;
	int status = 0;
	time_t start = time(NULL);
	
	arr[0] = first;
	
	status = KTRec(arr, board, 1, start); 
	
	if(time(NULL) - start > WID * HEI)
	{
		return T_FAIL;
	}
	
	return status;
}

int HasDuplicate(int *arr)
{
	int *table = (int*)calloc(WID * HEI, sizeof(int));
	int i = 0;
	
	for(i = 0; i < WID * HEI; ++i)
	{
		if(1 == table[arr[i]])
		{
			free(table);
			
			return 0;
		}	
		table[arr[i]] = 1;
	}
	
	free(table);
	
	return 1;
}

int Test(int start_point)
{
	int *arr = (int*)malloc(64 * sizeof(int));
	enum status status = 0;
	time_t start = time(NULL);
	int flag = 1;
	
	int i = 0;
	
	status = KT(arr, start_point);
	
	for(i = 0; i < WID * HEI - 1; ++i)
	{
		if(!IsLegalMove(arr[i], arr[i + 1]))
		{
			flag = 0;
		}
	}
	
	if(!HasDuplicate(arr))
	{
		flag = 0;
	}
	
	printf("Time: %lu:%lu\n", (time(NULL) - start) / 60, (time(NULL) - start) % 60);
	printf("Path: \n");
	
	PrintArr(arr);
	
	return flag;
}



int main(int argc, char *argv[])
{
	int value = 0;
	int res = 0;
	
	(void)value;
	
	srand(time(NULL));
	
	if(argc > 1)
	{
		value = atoi(argv[1]);
		printf("Result: %d\n", Test(value));
	}
	else
	{
		for(value = 0; value < 64; ++value)
		{
			printf("Calling with: %d\n", value);
			res = Test(value);
			printf("Result: %d\n", res);
			sleep(1);
			printf("\n\n");
		}
	}
	
	return 0;
}


#if 0

static int IsValid(size_t board, int width, int height, int x, int y)
{
    if (x >= 0 
        && y >= 0 
        && x < width
        && y < height 
        && BARRIsOff(board, GetIndex(x, y, width)))
    {
        return 1;
    }

    return 0;
}


static void InitLUT(void)
{
    int i = 0, j = 0, k = 0;
    int cur_x = 0, cur_y = 0;
    
    for(; i < 64; ++i)
    {
        k = 0;      

        for(j = 0; j < N_MOVES; ++j)
        {   
            cur_x = i % 8 + moves_x[j];
            cur_y = i / 8 + moves_y[j];

            if(IsValid(0, 8, 8, cur_x, cur_y))
            {
                moves[i][k] = cur_x + cur_y * 8;

                ++k;
            }
        }

        moves[i][k] = -1;
    }
}


void PrintLUT()

{
    int i = 0, j = 0;
    printf("{");

    for(; i < 64; ++i)
    {
        printf("{");
		
        for(j = 0; j < 8; ++j)
        {
            printf("%4d, ", moves[i][j]);
        }

        printf("%4d}, /*%d*/\n", moves[i][j], i);
    }

    printf("}\n");

}

int CountDegrees(unsigned char opt)
{
	int count = 0;

	while(moves[opt][count] > -1)
	{
		++count;
	}
	
	return count;
}


int SortByDegrees(const void *opt1, const void *opt2)
{
	return CountDegrees(*(char*)opt1) - CountDegrees(*(char*)opt2);
}


void SortLUT()
{
	int i = 0;
	int count = 0;
	
	for(i = 0; i < 64; ++i)
	{
		count = 0;
		while(moves[i][count] != -1)
		{
			++count;
		}
		BubbleSort(&moves[i][0], count, sizeof(char), SortByDegrees);
	}
	
	PrintLUT();
}



#endif


