#include "types.h"

u32 MapSelect_AddMapButtons(u32 co);
int MenuButtons_EnableTouch(int exp);

int func_02060f84(u32 co)
{
    return MenuButtons_EnableTouch(MapSelect_AddMapButtons(co));
}
