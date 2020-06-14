#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


size_t Strlen(const char *str)
int Strcmp(const char *s1, const char *s2)
char *Strcpy(char *dest, const char *src);
char *Strncpy(char *dest, const char *src, size_t n);
int Strcasecmp(const char *str1, const char *str2);
int Strcasecmp_First(const char *str1, const char *str2);
char *Strchr(const char *str, char c);
char *Strdup(const char *str);
char *Strcat(char *dest, const char *src);
char *Strstr(const char *big_str, const char *sml_str);
int Strspn(const char *s, const char *accept);
void SwapPointers(int **first, int **second);
void ClearSpaces(char *str);


