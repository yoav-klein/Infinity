#include <stdio.h>
#include <stdlib.h>

#define WIDTH (7)
#define HEIGHT (7)

enum { VAL, FLAG };

void FillMyMap(char map[HEIGHT][WIDTH], char my_map[HEIGHT][WIDTH][2])
{
	int i = 0;
	int j = 0;
	
	for(i = 0; i < HEIGHT; ++i)
	{
		for(j = 0; j < WIDTH; ++j)
		{
			my_map[i][j][VAL] = map[i][j];
			my_map[i][j][FLAG] = 0;
		}
	}
}

void SignRec(char my_map[HEIGHT][WIDTH][2], int i, int j)
{
	if (i < 0 || j < 0 || i >= HEIGHT || j >= WIDTH || my_map[i][j][VAL] == 0 ||
	my_map[i][j][FLAG] == 1)
	{
		return;
	}
	
	my_map[i][j][FLAG] = 1;
		
	SignRec(my_map, i - 1, j);
	SignRec(my_map, i + 1, j);
	SignRec(my_map, i, j + 1);
	SignRec(my_map, i, j - 1);											
}

int NumOfIslands(char map[HEIGHT][WIDTH])
{
	int count = 0;
	int i = 0;
	int j = 0;
	char my_map[HEIGHT][WIDTH][2];
	
	FillMyMap(map, my_map);
	
	for(i = 0; i < HEIGHT; ++i)
	{
		for(j = 0; j < WIDTH; ++j)
		{
			if(my_map[i][j][VAL] == 1 && my_map[i][j][FLAG] == 0)
			{
				++count;
				SignRec(my_map, i, j);
			}
		}
	}
	
	return count;
}

void CreateMap(char map[HEIGHT][WIDTH])
{
	int i = 0;
	int j = 0;
	
	for(i = 0; i < HEIGHT; ++i)
	{
		for(j = 0; j < WIDTH; ++j)
		{
			map[i][j] = rand() % 2;
		}
	}
}

void PrintMap(char map[HEIGHT][WIDTH])
{
	int i = 0;
	int j = 0;
	
	for(i = 0; i < HEIGHT; ++i)
	{
		for(j = 0; j < WIDTH; ++j)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	char map[HEIGHT][WIDTH];	
	
	CreateMap(map);
	
	PrintMap(map);
	
	
	
	printf("Num of islands: %d\n", NumOfIslands(map));
	
	return 0;
}



