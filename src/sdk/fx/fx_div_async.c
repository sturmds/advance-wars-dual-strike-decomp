#include "sdk/fx.h"

/* Starts numer / denom on the hardware divider; the quotient is
 * (numer << 12) / denom, read with FX_GetDivResult. */
void FX_DivAsync(fx32 numer, fx32 denom) {
  REG_DIVCNT = DIVCNT_MODE_64_32;
  IO_DIVIDER_DIVIDEND = (long long)numer << 32;
  REG_DIV_DENOM = (u32)denom; /* zero-extended: the high word is stored as 0 */
}
