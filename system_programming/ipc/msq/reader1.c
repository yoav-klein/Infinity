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

void Read(int msid)
{
	struct message_buf message = { 0 };
	
	if(-1 == msgrcv(msid, &message, sizeof(message.mesg_text), 2, 0))
	{
		perror("message recevie failed:");
	}
	
	/* should be the third message */
	printf("%s\n", message.mesg_text);
}

int main()
{
	key_t key = ftok(PATH, ID);
	int msid = msgget(key, 0666);
	
	if(-1 == msid)
	{
		perror("msgget failed:");
		exit(1);
	}
	
	Read(msid);
	
	return 0;
}
