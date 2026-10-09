#include "types.h"

s8 CallbackSlots_Add(const void *descriptor);
extern u8 func_ov003_02353ef4[];
extern u8 func_ov003_02353d14[];

void func_ov003_02353bd8(void)
{
    CallbackSlots_Add(func_ov003_02353ef4);
    CallbackSlots_Add(func_ov003_02353d14);
}
