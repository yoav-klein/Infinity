#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATH "writer.c"
#define ID (60)

struct message_buf
{
	long mesg_type; 
	char mesg_text[100]; 
};

int CreateQueue()
{
	int msid = 0;
	key_t key = 0;
	
	key = ftok(PATH, ID);
	
	msid = msgget(key, IPC_CREAT | 0666);
	if(-1 == msid)
	{
		perror("msgged failed");
		exit(1);
	}
	
	return msid;
}

void WriteFirstMessage(int msid)
{
	struct message_buf message = { 1, "This is the first message" };
	if(-1 == msgsnd(msid, &message, sizeof(message.mesg_text), 0))
	{
		perror("first send failed:");
	}
	
}

void WriteSecondMessage(int msid)
{
	struct message_buf message = { 1, "This is the second message" };
	if(-1 == msgsnd(msid, &message, sizeof(message.mesg_text), 0))
	{
		perror("second send failed:");
	}
}

void WriteThirdMessage(int msid)
{
	struct message_buf message = { 2, "This is the third message" };
	if(-1 == msgsnd(msid, &message, sizeof(message.mesg_text), 0))
	{
		perror("third send failed:");
	}
}

int main()
{
	int msid = 0;
	msid = CreateQueue();
	printf("Here %d\n", msid);
	WriteFirstMessage(msid);
	WriteSecondMessage(msid);
	WriteThirdMessage(msid);
	
	return 0;
}
