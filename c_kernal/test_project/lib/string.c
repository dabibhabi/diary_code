#include "string.h"

void* memset(void* dst, int c, size_t n) {
    // TODO: byte-by-byte fill; consider rep stosb optimization later
    (void)dst; (void)c; (void)n;
    return dst;
}

void* memcpy(void* dst, const void* src, size_t n) {
    // TODO: byte-by-byte copy; assumes non-overlapping buffers
    (void)dst; (void)src; (void)n;
    return dst;
}

int memcmp(const void* a, const void* b, size_t n) {
    // TODO: compare byte-by-byte, return (a[i] - b[i]) on first mismatch
    (void)a; (void)b; (void)n;
    return 0;
}

size_t strlen(const char* s) {
    // TODO: walk until '\0', return count
    (void)s;
    return 0;
}

int strcmp(const char* a, const char* b) {
    // TODO: compare until mismatch or joint '\0'
    (void)a; (void)b;
    return 0;
}
