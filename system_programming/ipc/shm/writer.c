#define _GNU_SOURCE

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define PATH "writer.c"
#define ID (65)

void WriteToSharedMemory()
{
	char *shmem = NULL;
	
	key_t key;
	int shmid = 0;
	
	key = ftok(PATH, ID);
	shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | IPC_PRIVATE | 0640);
	
	shmem = shmat(shmid, NULL, 0);
	printf("%p\n", shmem);
	sprintf(shmem, "Hello there, i'm %d years old\0", 3);
	
	shmdt(shmem);
	
}

int main()
{
	WriteToSharedMemory();	
	
	
	return 0;
}

