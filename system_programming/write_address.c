#include <stdio.h>
#include <unistd.h>

int main()
{
	FILE *fp;
	int a = 10;
	int *ptr = &a;
	
	fp = fopen("MyFile", "w");
	
	fwrite(&ptr, 8, 1, fp);
	
	printf("Address: %p\n", ptr);
	
	fclose(fp);
	
	sleep(60);
	
	return 1;
}
