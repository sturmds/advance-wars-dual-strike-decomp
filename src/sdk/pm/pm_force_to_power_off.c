#include "sdk/pm.h"

int func_02114e04(void (*callback)(u32 result, u32 *dest), u32 *dest);
void Power_SpinUntilIdle(void);

int PM_ForceToPowerOff(void)
{
    u32 result;
    int error;

    error = func_02114e04(Power_SaveSyncResult, &result);
    if (error != 0) {
        return error;
    }
    Power_SpinUntilIdle();
    return result;
}
