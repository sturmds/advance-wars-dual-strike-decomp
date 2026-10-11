#include "sdk/fx.h"

/* Starts 1.0 / denom on the hardware divider; read with FX_GetDivResult. */
void FX_InvAsync(fx32 denom)
{
    *REG_DIVCNT = DIVCNT_MODE_64_32;
    *REG_DIV_NUMER = (long long)0x1000 << 32;
    *REG_DIV_DENOM = (u32)denom;           /* zero-extended: the high word is stored as 0 */
}
