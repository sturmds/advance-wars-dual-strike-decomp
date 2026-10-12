#include "types.h"
#include "sdk/pm.h"

/* Cuts power to the wireless hardware and sets the power LED back to steady. */
void func_arm7_03805878(void) {
  *REG_POWCNT2 &= ~POWCNT2_WIRELESS;
  PowerLed_SelectPattern(PM_LED_ON);
  PMi_SetLED(PM_LED_ON);
}
