#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int func_02066b18(int exp);

int func_020678e0(u32 co)
{
    return func_02066b18(MenuButtons_EnableTouch(co));
}
