#include "sdk/gx.h"

BOOL Power_LcdsPowered(void)
{
    return (*REG_POWCNT1 & POWCNT1_LCD) != 0;
}
