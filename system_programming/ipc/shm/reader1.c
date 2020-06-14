#define _GNU_SOURCE

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define PATH "writer.c"
#define ID (65)

void ReadFromSharedMemory()
{
	char *shmem = NULL;
	
	key_t key;
	int shmid = 0;
	
	key = ftok(PATH, ID);
	shmid = shmget(key, 4096, IPC_CREAT | 0640);
	
	shmem = shmat(shmid, NULL, 0);
	printf("address: %p\n", shmem);
	
	printf("%s\n", shmem);
	
	shmdt(shmem);
	
}

int main()
{
	ReadFromSharedMemory();	
	
	
	return 0;
}

