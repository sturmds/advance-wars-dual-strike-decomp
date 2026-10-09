#include "types.h"

u32 CallbackSlots_Clear(u32 co);
int func_02028d60(int exp);

int func_020d80a8(u32 co)
{
    return func_02028d60(CallbackSlots_Clear(co));
}
