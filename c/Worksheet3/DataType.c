#include <stdio.h> /* printf */

void PrintTypeSizes()
{
	
	printf("char: %ld\n", sizeof(char));
	printf("unsigned char: %ld\n", sizeof(unsigned char));
	printf("int: %ld\n", sizeof(int));
	printf("unsigned int: %ld\n", sizeof(unsigned int));
	printf("short: %ld\n", sizeof(short));
	printf("unsigned short: %ld\n", sizeof(unsigned short));
	printf("long: %ld\n", sizeof(long));
	printf("unsigned long: %ld\n", sizeof(unsigned long));
	printf("float: %ld\n", sizeof(float));
	printf("double: %ld\n", sizeof(double));
	printf("long double: %ld\n", sizeof(long double));
	printf("char *: %ld\n", sizeof(char*));

}

int main()
{
PrintTypeSizes();

return 0;
}
