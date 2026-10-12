#include "types.h"

void FX_DivAsync(fx32 numer, fx32 denom);
fx32 FX_GetDivResult(void);

/* numer / denom in fx32 on the hardware divider (DIVCNT at 0x04000280). */
fx32 FX_Div(fx32 numer, fx32 denom) {
  FX_DivAsync(numer, denom);
  return FX_GetDivResult();
}
