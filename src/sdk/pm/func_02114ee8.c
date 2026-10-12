#include "sdk/pm.h"

int func_02114ee8(int led)
{
    u32 result;
    int error;

    error = Power_RequestLedState(led, Power_SaveSyncResult, &result);
    if (error != 0) {
        return error;
    }
    Power_SpinUntilIdle();
    return result;
}
