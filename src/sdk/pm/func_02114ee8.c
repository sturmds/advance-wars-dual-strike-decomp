#include "sdk/pm.h"

int Power_SendLedStateAsync(int sw, void (*callback)(u32 result, u32 *dest), u32 *dest);
void Power_SpinUntilIdle(void);

int func_02114ee8(int sw)
{
    u32 result;
    int error;

    error = Power_SendLedStateAsync(sw, Power_SaveSyncResult, &result);
    if (error != 0) {
        return error;
    }
    Power_SpinUntilIdle();
    return result;
}
