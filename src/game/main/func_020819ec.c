#include "types.h"

u32 func_02081a08(u32 co);
int ExperienceToRank(int exp);

int func_020819ec(u32 co)
{
    return ExperienceToRank(func_02081a08(co));
}
