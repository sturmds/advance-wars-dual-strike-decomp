#include "types.h"
#include "sdk/pm.h"

BOOL func_arm7_03803d38(int type, int command, int count, int arg);

/* Patterns 4 to POWER_LIGHT_LAST_PATTERN. */
extern PowerLedBlinkScript gPowerLedBlinkTable[];

/* Advances the current LED pattern by one tick and queues an LED change when the state differs. */
void PowerLed_StepPattern(void)
{
    int pattern = data_arm7_0380a02c;
    PowerLedBlinkScript *entry;
    u32 bit;
    int status;

    if (pattern == 0) {
        if (func_arm7_03803d38(SERIAL_JOB_POWER_CHIP, POWER_MSG_LED_STATE, 1, PM_LED_ON)) {
            PowerLed_SelectPattern(PM_LED_ON);
        }
        return;
    }
    if (pattern < 4) {
        /* patterns 1 to 3 are the LED states themselves */
        if (pattern != gPowerLedLastState) {
            func_arm7_03803d38(SERIAL_JOB_POWER_CHIP, POWER_MSG_LED_STATE, 1, pattern);
        }
        return;
    }

    entry = &gPowerLedBlinkTable[pattern - 4];
    bit = gPowerLedStepCounter / entry->framesPerStep;
    status = (entry->bits & (0x8000000000000000ULL >> bit)) ? PM_LED_ON : POWER_LIGHT_SLOW_FLASH;
    if (++gPowerLedStepCounter >= entry->stepCount * entry->framesPerStep) {
        gPowerLedStepCounter = 0;
    }
    if (status != gPowerLedLastState) {
        func_arm7_03803d38(SERIAL_JOB_POWER_CHIP, POWER_MSG_LED_STATE, 1, status);
    }
}
