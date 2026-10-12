#include "types.h"
#include "sdk/pm.h"

/* Sets bits in the power chip control register. */
void PMi_SetControl(int bits)
{
    int value = PMi_GetRegister(POWER_SWITCHES_REG_INDEX);
    value |= bits;
    PMi_SetRegister(POWER_SWITCHES_REG_INDEX, value);
}
