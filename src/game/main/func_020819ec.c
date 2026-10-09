#include "types.h"

u32 Co_GetExperienceFiltered(u32 co);
int ExperienceToRank(int exp);

int func_020819ec(u32 co)
{
    return ExperienceToRank(Co_GetExperienceFiltered(co));
}
