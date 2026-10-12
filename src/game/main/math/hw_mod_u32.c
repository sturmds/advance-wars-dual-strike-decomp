#include "types.h"
#include "hw_div.h"

/* Unsigned remainder: both operands are zero-extended to 64 bits. */
u32 HwModU32(u32 numer, u32 denom) {
  REG_DIVCNT = DIVCNT_MODE_64_64;
  IO_DIVIDER_DIVIDEND = numer;
  REG_DIV_DENOM = denom;
  while (REG_DIVCNT & DIVCNT_BUSY) {
  }
  return REG_DIVREM_RESULT;
}
