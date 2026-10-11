#include "sdk/pm.h"

int func_02114f24(int sw, void (*callback)(u32 result, u32 *dest), u32 *dest);
void func_02115378(void);
void func_02115370(u32 result, u32 *dest);

int func_02114ee8(int sw)
{
    u32 result;
    int error;

    error = func_02114f24(sw, func_02115370, &result);
    if (error != 0) {
        return error;
    }
    func_02115378();
    return result;
}
