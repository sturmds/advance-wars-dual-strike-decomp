#include "types.h"

u32 MenuButtons_EnableTouch(u32 co);
int VsSetup_AddPlayerButtons(int exp);

int func_020678e0(u32 co)
{
    return VsSetup_AddPlayerButtons(MenuButtons_EnableTouch(co));
}
