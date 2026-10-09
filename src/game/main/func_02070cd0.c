#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int func_02070250(int exp);

int func_02070cd0(u32 co)
{
    return func_02070250(MenuButtons_EnableTouch(co));
}
