#include "sdk/pm.h"

int Power_PostCommandWithReply(u32 command, PMCallback callback, u32 *arg);

static inline int GetBackLightCommand(int backlight)
{
    switch (backlight) {
    case 1:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

/* backlight 1, 2 and 3 are sent as the commands 1, 3 and 2. 0xffff: bad argument. */
int Power_SendLedStateAsync(int backlight, PMCallback callback, u32 *arg)
{
    int command = GetBackLightCommand(backlight);

    if (command == 0) {
        return 0xffff;
    }
    return Power_PostCommandWithReply(command, callback, arg);
}
