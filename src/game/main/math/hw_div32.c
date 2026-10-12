#include "types.h"
#include "hw_div.h"

s32 HwDiv32(s32 numer, s32 denom) {
  REG_DIVCNT = DIVCNT_MODE_32_32;
  REG_DIV_NUMER = numer;
  /* The high word is written as 0, not sign-extended; mode 0 ignores it. */
  REG_DIV_DENOM = (u32)denom;
  while (REG_DIVCNT & DIVCNT_BUSY) {
  }
  return REG_DIV_RESULT;
}
