#include "types.h"
#include "sdk/fx.h"

/* numer / denom in fx32 on the hardware divider (DIVCNT at 0x04000280). */
fx32 FX_Div(fx32 numer, fx32 denom) {
  FX_DivAsync(numer, denom);
  return FX_GetDivResult();
}
