#include "types.h"

u32 Overlay_SwapSlot1ForSlot3(u32 co);
int Ov003_Init(int exp);

int func_02083020(u32 co)
{
    return Ov003_Init(Overlay_SwapSlot1ForSlot3(co));
}
