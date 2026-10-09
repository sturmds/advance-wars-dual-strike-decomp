#include "types.h"

u32 func_02039f9c(u32 co);
int Mem_Free(int exp);

int func_02039f80(u32 co)
{
    return Mem_Free(func_02039f9c(co));
}
