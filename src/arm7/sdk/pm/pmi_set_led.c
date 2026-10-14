#include "types.h"
#include "sdk/pm.h"
#include "sdk/os_arm7.h"

int gPowerLedLastState = {
#include "data/arm7/gPowerLedLastState.inc.c"
};
/* declared `PowerLedBlinkScript gPowerLedBlinkTable[]` in power_led_step_pattern.c: PowerLedBlinkScript.bits has type u64 */
u32 gPowerLedBlinkTable[36] = {
#include "data/arm7/gPowerLedBlinkTable.inc.c"
};

/* Sets the power LED to steady, slow blink or fast blink; any other state halts. */
void PMi_SetLED(int status) {
  switch (status) {
    case PM_LED_ON:
      PMi_ResetControl(POWER_SWITCH_LED_FLASHING);
      break;
    case POWER_LIGHT_QUICK_FLASH:
      PMi_SetControl(POWER_SWITCH_LED_FLASHING | POWER_SWITCH_LED_QUICK_RATE);
      break;
    case POWER_LIGHT_SLOW_FLASH:
      PMi_ResetControl(POWER_SWITCH_LED_QUICK_RATE);
      PMi_SetControl(POWER_SWITCH_LED_FLASHING);
      break;
    default:
      Cpu_HaltForever_Arm7();
  }
  gPowerLedLastState = status;
}
