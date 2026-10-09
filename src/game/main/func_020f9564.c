#include "types.h"

u32 Screen_SetLayerPriorities(u32 co);
int Screen_ResetBlend(int exp);

int func_020f9564(u32 co)
{
    return Screen_ResetBlend(Screen_SetLayerPriorities(co));
}
