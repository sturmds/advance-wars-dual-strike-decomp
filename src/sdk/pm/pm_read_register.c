#include "sdk/pm.h"

int PMi_ReadRegisterAsync(u32 reg, u16 *data, PMCallback callback, u32 *arg);
void Power_SpinUntilIdle(void);

/* Reads PM register `reg` into *data and waits for it. Returns 0, or the error of the command. */
int PMi_ReadRegister(u32 reg, u16 *data)
{
    u32 result;
    int error;

    error = PMi_ReadRegisterAsync(reg, data, Power_SaveSyncResult, &result);
    if (error != 0) {
        return error;
    }
    Power_SpinUntilIdle();
    return result;
}
