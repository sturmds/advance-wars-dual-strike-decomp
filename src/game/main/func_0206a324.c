#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int func_020682f8(int exp);

int func_0206a324(u32 co)
{
    return func_020682f8(MenuButtons_EnableTouch(co));
}
