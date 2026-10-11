#include "sdk/fx.h"

/* Waits for the hardware divider, then returns the quotient rounded to
 * fx32. */
fx32 FX_GetDivResult(void)
{
    while (REG_DIVCNT & DIVCNT_BUSY) {
    }
    return (fx32)((IO_DIVIDER_QUOTIENT + 0x80000) >> 20);
}
