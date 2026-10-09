#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int VsSetup_AddMapAndCoButtons(int exp);

int func_0206a324(u32 co)
{
    return VsSetup_AddMapAndCoButtons(MenuButtons_EnableTouch(co));
}
