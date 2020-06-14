#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */
#include <sys/ipc.h> /* ftok */
#include <unistd.h> /* getcwd */
#include <errno.h> /* perror */
#include <stdlib.h> /* malloc */ 

#define IDENTIFIER (69)
#define SEM_PERMS (0600)
#define INIT_SEM_VAL (2)

enum semaphores { SEM_PING, SEM_PONG, NUM_SEMS };

void SemInfo()
{
	key_t key = 0;
	char *path = "pong.out";
	int semid = 0;
	
	key = ftok(path, IDENTIFIER);	
	
	semid = semget(key, NUM_SEMS, IPC_CREAT | SEM_PERMS);
	
	printf("SEM_PING: %d, SEM_PONG: %d\n", semctl(semid, SEM_PING, GETVAL), 
	semctl(semid, SEM_PONG, GETVAL));
}

int main()
{
	SemInfo();
	
	return 0;
}
