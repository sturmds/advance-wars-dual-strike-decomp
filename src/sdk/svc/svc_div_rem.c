#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x09: signed division; returns the remainder, which the BIOS
   leaves in r1. */
#pragma thumb on
asm int SVC_DivRem(int numerator, int denominator)
{
    swi 0x09
    mov r0, r1
    bx lr
}
#pragma thumb reset
#else
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* The remainder, with the sign of the numerator. C leaves a zero
   denominator and INT_MIN % -1 undefined, so those stop the program. */
int SVC_DivRem(int numerator, int denominator) {
  if (denominator == 0 || (numerator == INT_MIN && denominator == -1)) {
    fprintf(stderr, "SVC_DivRem: %d %% %d has no defined C result\n",
            numerator, denominator);
    abort();
  }
  return numerator % denominator;
}
#endif
