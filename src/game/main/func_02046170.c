#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int func_020458e0(int exp);

int func_02046170(u32 co)
{
    return func_020458e0(MenuButtons_EnableTouch(co));
}
