#include <stddef.h>
#include <stdio.h>
#define WORD_SIZE 8
#define Test(x) if(x() == 1) printf(#x " OK!\n")

void *Memset(void *s, int c, size_t n);
void *Memcpy(void *dest, void *src, size_t n);
void *Memmove(void* dest, const void *src, size_t n);
int Memset_Test();
int Memcpy_Test();
int Memmove_Test();
