#include "types.h"

u32 func_020f9ca0(u32 co);
int Screen_ResetBlend(int exp);

int func_020f9838(u32 co)
{
    return Screen_ResetBlend(func_020f9ca0(co));
}
