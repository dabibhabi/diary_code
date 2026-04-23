#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

// TODO: freestanding integer typedefs (uint8_t..uint64_t, size_t, bool)
// NOTE: no libc available — do not include <stdint.h> unless you have a
//       freestanding toolchain that provides it.

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

// TODO: pick size_t width based on target (ILP32 vs LP64)
typedef uint32_t           size_t;

#define NULL ((void*)0)

#endif
