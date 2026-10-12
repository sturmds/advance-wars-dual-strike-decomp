#include "types.h"
#include "sdk/pm.h"

void SndHw_Disable(void);

/* Applies one POWER_ORDER_* from the ARM9 to the LED, backlights, sound amplifier or power. */
void Power_ApplyArm9Command(int command)
{
    switch (command) {
    case POWER_ORDER_LED_STEADY:
        PowerLed_SelectPattern(PM_LED_ON);
        PMi_SetLED(PM_LED_ON);
        break;
    case POWER_ORDER_LED_QUICK_FLASH:
        PowerLed_SelectPattern(POWER_LIGHT_QUICK_FLASH);
        PMi_SetLED(POWER_LIGHT_QUICK_FLASH);
        break;
    case POWER_ORDER_LED_LAZY_FLASH:
        PowerLed_SelectPattern(POWER_LIGHT_SLOW_FLASH);
        PMi_SetLED(POWER_LIGHT_SLOW_FLASH);
        break;
    case POWER_ORDER_LOWER_LCD_LIT:
        PMi_SetControl(POWER_SWITCH_LOWER_LAMP);
        break;
    case POWER_ORDER_LOWER_LCD_DARK:
        PMi_ResetControl(POWER_SWITCH_LOWER_LAMP);
        break;
    case POWER_ORDER_UPPER_LCD_LIT:
        PMi_SetControl(POWER_SWITCH_UPPER_LAMP);
        break;
    case POWER_ORDER_UPPER_LCD_DARK:
        PMi_ResetControl(POWER_SWITCH_UPPER_LAMP);
        break;
    case POWER_ORDER_BOTH_LCDS_LIT:
        PMi_SetControl(POWER_SWITCH_LOWER_LAMP | POWER_SWITCH_UPPER_LAMP);
        break;
    case POWER_ORDER_BOTH_LCDS_DARK:
        PMi_ResetControl(POWER_SWITCH_LOWER_LAMP | POWER_SWITCH_UPPER_LAMP);
        break;
    case POWER_ORDER_AMPLIFIER_ENABLE:
        PMi_SetControl(POWER_SWITCH_AMPLIFIER);
        break;
    case POWER_ORDER_AMPLIFIER_DISABLE:
        PMi_ResetControl(POWER_SWITCH_AMPLIFIER);
        break;
    case POWER_ORDER_SILENCE_SPEAKER:
        PMi_SetControl(POWER_SWITCH_SILENCE);
        break;
    case POWER_ORDER_UNSILENCE_SPEAKER:
        PMi_ResetControl(POWER_SWITCH_SILENCE);
        break;
    case POWER_ORDER_CANCEL_SHUTDOWN:
        PMi_ResetControl(POWER_SWITCH_CUT_SUPPLY);
        break;
    case POWER_ORDER_SHUT_DOWN:
        SndHw_Disable();
        PMi_ResetControl(POWER_SWITCH_AMPLIFIER);
        PMi_SetControl(POWER_SWITCH_CUT_SUPPLY);
        break;
    }
}
