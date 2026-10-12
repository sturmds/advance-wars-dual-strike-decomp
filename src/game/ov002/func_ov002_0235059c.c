#include "types.h"

u32 Battle_ResetScreen(u32 co);
int Battle_Init(int exp);

int func_ov002_0235059c(u32 co) { return Battle_Init(Battle_ResetScreen(co)); }
