#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed long long s64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef int BOOL;

/* Fixed point with 12 fraction bits: 1.0 is 0x1000. */
typedef s32 fx32;
#define FX32_ONE 0x1000

#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0

#endif
