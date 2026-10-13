#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x09: signed division; returns the quotient. */
#pragma thumb on
asm int Bios_Quotient_Arm7(int numerator, int denominator)
{
    swi 0x09
    bx lr
}
#pragma thumb reset
#else
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* The quotient, rounded towards zero. C leaves a zero denominator and
   INT_MIN / -1 undefined, so those stop the program. */
int Bios_Quotient_Arm7(int numerator, int denominator) {
  if (denominator == 0 || (numerator == INT_MIN && denominator == -1)) {
    fprintf(stderr, "Bios_Quotient_Arm7: %d / %d has no defined C result\n",
            numerator, denominator);
    abort();
  }
  return numerator / denominator;
}
#endif
