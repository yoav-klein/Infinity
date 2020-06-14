#include <stdio.h> /* printf */
#include <sys/types.h> /* ftok */
#include <sys/ipc.h> /* ftok */
#include <sys/sem.h> /* semget */
#include <unistd.h> /* sleep */

#define PATH "waiter.out"
#define IDENTIFIER (54)
#define NUM_SEMS (1)
#define SEM_PERMS (0600)


int InitSemaphore()
{
	int semid = 0;
	key_t key;
	
	key = ftok(PATH, IDENTIFIER);
	if(-1 == key)
	{
		perror("ftok failed");
		
		return -1;
	}
	
	semid = semget(key, NUM_SEMS, SEM_PERMS | IPC_CREAT);
	if(-1 == semid)
	{
		perror("semget failed");
		
		return -1;
	}
	
	if(-1 == semctl(semid, 0, SETVAL, 1))
	{
		perror("setval failed");
		
		return -1;
	}
	
	return semid;
}

void DecrementOne(int semid)
{
	struct sembuf buf;
	
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	
	if(-1 == semop(semid, &buf, 1))
	{
		perror("decrementing semaphore failed");
		
		return;
	}
}

void RaiseWaiter()
{
	pid_t pid;
	
	pid = fork();
	
	if(!pid)
	{
		execl(PATH, PATH, NULL);
	}
}	

int main()
{
	int semid = InitSemaphore();
	
	printf("quitter: semid - %d\n", semid);
	DecrementOne(semid);
	printf("quitter: decremented one\n");

	RaiseWaiter();
	printf("quitter: raised waiter\n");
	sleep(5);
}


