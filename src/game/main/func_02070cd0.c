#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int TabList_RebuildButtons(int exp);

int func_02070cd0(u32 co)
{
    return TabList_RebuildButtons(MenuButtons_EnableTouch(co));
}
