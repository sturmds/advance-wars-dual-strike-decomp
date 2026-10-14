#ifndef SDK_SETJMP_H
#define SDK_SETJMP_H

#include "types.h"

/* The buffer of setjmp and longjmp (src/sdk/msl). setjmp saves ten words
 * (r4-r11, sp and lr); the buffer has room for 16. */
typedef u32 jmp_buf[16];

#endif
