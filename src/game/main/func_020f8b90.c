#include "types.h"

u32 Screen_ResetBlend(u32 co);
int func_020097c8(int exp);

int func_020f8b90(u32 co)
{
    return func_020097c8(Screen_ResetBlend(co));
}
