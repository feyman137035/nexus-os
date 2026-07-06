#include "string.h"

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    if (n == 0) return 0;
    while (n > 1 && *str1 && (*str1 == *str2)) {
        str1++;
        str2++;
        n--;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n > 0) {
        *p = (unsigned char)c;
        p++;
        n--;
    }
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n > 0) {
        *d = *s;
        d++;
        s++;
        n--;
    }
    return dest;
}
