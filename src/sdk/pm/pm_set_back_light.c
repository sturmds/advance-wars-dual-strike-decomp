#include "sdk/pm.h"

int PM_SetBackLightAsync(int target, int order, void (*callback)(u32 result, u32 *dest), u32 *dest);
void func_02115378(void);
void func_02115370(u32 result, u32 *dest);

int PM_SetBackLight(int target, int order)
{
    u32 result;
    int error;

    error = PM_SetBackLightAsync(target, order, func_02115370, &result);
    if (error != 0) {
        return error;
    }
    func_02115378();
    return result;
}
