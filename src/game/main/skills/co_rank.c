#include "types.h"

u32 Co_GetExperience(
    u32 co); /* CO's experience points; 0 for a clone (co & 0x80) */
int ExperienceToRank(int exp); /* experience to rank: exp / 1000, at most 100 */

/* Rank of CO `co`: its saved experience / 1000, at most 100. */
int Co_GetRank(u32 co) { return ExperienceToRank(Co_GetExperience(co)); }
