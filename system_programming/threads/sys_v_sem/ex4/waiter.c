#include <stdio.h> /* printf */
#include <sys/types.h> /* ftok */
#include <sys/ipc.h> /* ftok */
#include <sys/sem.h> /* semget */

#define PATH "waiter.out"
#define IDENTIFIER (54)
#define NUM_SEMS (1)
#define SEM_PERMS (0600)

void WaitForHimToDie(int semid)
{
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	
	printf("Waiting for him to die\n");
	if(-1 == semop(semid, &buf, 1))
	{
		perror("waiter: semop failed");
	}
}

int InitSemaphore()
{
	int semid = 0;
	key_t key;
	
	key = ftok(PATH, IDENTIFIER);
	if(-1 == key)
	{
		perror("waiter: ftok failed");
	}
	
	semid = semget(key, NUM_SEMS, SEM_PERMS | IPC_CREAT);
	if(-1 == semid)
	{
		perror("waiter: semget failed");
	}
}

int main()
{
	int semid = InitSemaphore();
	
	WaitForHimToDie(semid);
	
	printf("Apperantely he died!\n");
}
