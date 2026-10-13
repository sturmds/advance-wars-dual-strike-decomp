#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x0d: integer square root. */
#pragma thumb on
asm u32 SVC_Sqrt(u32 value)
{
    swi 0x0d
    bx lr
}
#pragma thumb reset
#else
/* The square root rounded down, found one result bit at a time from the
   top. */
u32 SVC_Sqrt(u32 value) {
  u32 root = 0;
  u32 bit = 1u << 30;

  while (bit > value) {
    bit >>= 2;
  }
  while (bit != 0) {
    if (value >= root + bit) {
      value -= root + bit;
      root = (root >> 1) + bit;
    } else {
      root >>= 1;
    }
    bit >>= 2;
  }
  return root;
}
#endif
