#include "types.h"
#include "sdk/pm.h"

/* Selects an LED pattern and restarts it from its first step; patterns above POWER_LIGHT_LAST_PATTERN are ignored. */
void PowerLed_SelectPattern(int pattern)
{
    if (pattern <= POWER_LIGHT_LAST_PATTERN) {
        data_arm7_0380a02c = pattern;
        gPowerLedStepCounter = 0;
    }
}
