#include "types.h"

u32 func_ov002_02357124(u32 co);
int Battle_Init(int exp);

int func_ov002_0235059c(u32 co)
{
    return Battle_Init(func_ov002_02357124(co));
}
