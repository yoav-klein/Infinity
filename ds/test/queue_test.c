#include <stdio.h>

#include "../include/queue.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }
					
int FlowTest()
{
	int flag = 1;
	int result = 0;
	int arr[5] = {1, 2, 3, 4, 5};
	int i = 0;
	queue_t *queue = QueueCreate();
	queue_t *second_queue = QueueCreate();
	
	
	result = QueueIsEmpty(queue);
	if(1 != result)
	{
		flag = 0;
	}
	
	/* Enqueue 5 items in both queues */
	for(i = 0; i < 5; ++i)
	{
		QueueEnqueue(queue, &arr[i]);
		QueueEnqueue(second_queue, &arr[i]);
	}
	
	/* Peek */
	result = *(int*)QueuePeek(queue);
	if(1 != result)
	{
		flag = 0;
	}
	
	
	/* Size */
	result = (int)QueueSize(queue);
	if(5 != result)
	{
		flag = 0;
	}
	
	/* IsEmpty */
	result = QueueIsEmpty(queue);
	if(0 != result)
	{
		
		flag = 0;
	}
	
	/*  Dequeue and peek */
	QueueDequeue(queue);
	
	result = *(int*)QueuePeek(queue);
	if(2 != result)
	{
		flag = 0; 
	}
	
	/*  Size */
	result = (int)QueueSize(queue);
	if(4 != result)
	{
		flag = 0; 
	}
	
	/* Append */
	queue = QueueAppend(queue, second_queue);
	
	/* size of appended queue should be 9 */
	result = (int)QueueSize(queue);
	if(9 != result)
	{
		flag = 0;
	}
	
	/* peek, should be 2 */
	result = *(int*)QueuePeek(queue);
	if(2 != result)
	{
	 	flag = 0; 
	}

	/* dequeue 4 times, then should be 1 */
	QueueDequeue(queue);
	QueueDequeue(queue);
	QueueDequeue(queue);
	QueueDequeue(queue);
	
	/* peek, should be 1 */
	result = *(int*)QueuePeek(queue);
	if(1 != result)
	{
		flag = 0;
	}
	
	
	return flag;
}
					
			
int main()
{
	RUN_TEST(FlowTest);
	
	return 1;
}
