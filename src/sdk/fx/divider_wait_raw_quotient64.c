#include "sdk/fx.h"

/* Waits for the hardware divider, then returns the raw 64-bit quotient. */
long long Divider_WaitRawQuotient64(void) {
  while (REG_DIVCNT & DIVCNT_BUSY) {
  }
  return IO_DIVIDER_QUOTIENT;
}
