#include "types.h"
#include "sdk/pm.h"

/* Clears bits in the power chip control register. */
void PMi_ResetControl(int bits) {
  u8 value = PMi_GetRegister(POWER_SWITCHES_REG_INDEX);
  value &= ~bits;
  PMi_SetRegister(POWER_SWITCHES_REG_INDEX, value);
}
