#include "types.h"

u32 Screen_ResetBlend(u32 co);
int Screen_ResetWindows(int exp);

int func_020f8b90(u32 co)
{
    return Screen_ResetWindows(Screen_ResetBlend(co));
}
