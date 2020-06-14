/************************************************
*		Binary blocking semaphore		
* 		Author: Yoav Klein
*
**************************************************/

#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */
#include <sys/ipc.h> /* semget */
#include <sys/types.h> /* semget */
#include <unistd.h> /* fork */
#include <stdlib.h> /* exit */ 

#define PATH "lock.c"
#define IDENTIFIER 33
#define NUM_SEMS 1
#define SEM_PERMS (0600)

int g_semid;

void InitSemaphore()
{
	key_t key = 0;
	
	key = ftok(PATH, IDENTIFIER);
	g_semid = semget(key, NUM_SEMS, IPC_CREAT | SEM_PERMS);
	if(-1 == g_semid)
	{
		perror("semget failed");
		
		exit(1);
	}
	
	if(-1 == semctl(g_semid, 0, SETVAL, 1))
	{
		perror("semctl failed");
		
		exit(1);
	}
}

void CreateChild()
{
	struct sembuf wait_buf = { 0 };
	struct sembuf post_buf = { 0 };
	pid_t child_pid = 0;
	
	wait_buf.sem_num = 0;
	wait_buf.sem_op = -1;
	wait_buf.sem_flg = 0;
	
	post_buf.sem_num = 0;
	post_buf.sem_op = 1;
	wait_buf.sem_flg = 0;
	
	child_pid = fork();
	while(1)
	{
		if(child_pid)
		{
			if(-1 == semop(g_semid, &wait_buf, 1))
			{
				perror("semop failed");
			}
			
			printf("Parent\n");
			
			if(-1 == semop(g_semid, &post_buf, 1))
			{
				perror("semop failed");
			}
		}
		else
		{
			if(-1 == semop(g_semid, &wait_buf, 1))
			{
				perror("semop failed");
			}
			
			printf("Child\n");
			
			if(-1 == semop(g_semid, &post_buf, 1))
			{
				perror("semop failed");
			}
		}
	}
}

int main()
{
	InitSemaphore();
	
	CreateChild();	
	
	return 0;
}



