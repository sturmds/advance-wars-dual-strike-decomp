#include "types.h"

int Player_GetCoRepairBonus(void);

int func_020e59fc(void)
{
    int t0 = Player_GetCoRepairBonus();
    return t0 + 2;
}
