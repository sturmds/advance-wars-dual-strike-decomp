#include "sdk/pm.h"

int Power_PostCommandWithReply(u32 command, PMCallback callback, u32 *arg);

static inline int GetLedCommand(int led)
{
    switch (led) {
    case 1:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

/* Sets the power LED: states 1, 2 and 3 (POWER_LIGHT_*) are sent as the commands 1, 3 and 2. 0xffff: bad argument. */
int Power_RequestLedState(int led, PMCallback callback, u32 *arg)
{
    int command = GetLedCommand(led);

    if (command == 0) {
        return 0xffff;
    }
    return Power_PostCommandWithReply(command, callback, arg);
}
