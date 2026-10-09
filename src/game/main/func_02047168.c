#include "types.h"

u32 func_02046dc4(u32 co);
int MenuButtons_EnableTouch(int exp);

int func_02047168(u32 co)
{
    return MenuButtons_EnableTouch(func_02046dc4(co));
}
