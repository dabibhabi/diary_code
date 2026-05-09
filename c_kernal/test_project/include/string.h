#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include "types.h"

// Freestanding string/memory helpers. GCC may emit calls to these even
// when you don't reference them directly (memcpy/memset intrinsics).

// TODO
void*  memset(void* dst, int c, size_t n);
// TODO
void*  memcpy(void* dst, const void* src, size_t n);
// TODO
int    memcmp(const void* a, const void* b, size_t n);
// TODO
size_t strlen(const char* s);
// TODO
int    strcmp(const char* a, const char* b);

#endif
