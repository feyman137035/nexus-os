#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif // STRING_H
