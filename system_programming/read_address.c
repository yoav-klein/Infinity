#include <stdio.h>

int main()
{
	FILE *fp;
	
	int *ptr = NULL;
	
	fp = fopen("MyFile", "r");
	
	fread(&ptr, 8, 1, fp);
	
	printf("Address: %p\n", ptr);
	printf("Value: %d\n", *ptr);
	
	fclose(fp);
	
	return 1;
}
