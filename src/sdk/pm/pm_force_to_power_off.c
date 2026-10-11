#include "sdk/pm.h"

int func_02114e04(void (*callback)(u32 result, u32 *dest), u32 *dest);
void Power_SpinUntilIdle(void);
void func_02115370(u32 result, u32 *dest);

int PM_ForceToPowerOff(void)
{
    u32 result;
    int error;

    error = func_02114e04(func_02115370, &result);
    if (error != 0) {
        return error;
    }
    Power_SpinUntilIdle();
    return result;
}
