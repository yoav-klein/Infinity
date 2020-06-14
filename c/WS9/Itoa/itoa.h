#include <stdio.h>

#define Test(x) if(x()) printf(#x " OK!")
#define EOS '\0'
#define ASCI_LETTER 87 
#define ASCI_DIGIT 48

char *IntToString(int base, int num, char *str);
int StringToInt(int base, char *str);
int StringToInt_Test();
int IntToString_Test();
