#include "types.h"

u32 func_02017a28(u32 co); /* CO's experience points; 0 for a clone (co & 0x80) */
int func_020e5450(int exp); /* experience to rank: exp / 1000, at most 100 */

/* Rank of CO `co`: its saved experience / 1000, at most 100. */
int Co_GetRank(u32 co)
{
    return func_020e5450(func_02017a28(co));
}
