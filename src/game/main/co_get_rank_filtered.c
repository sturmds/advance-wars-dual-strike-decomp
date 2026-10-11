#include "types.h"

int Co_GetExperienceFiltered(int coId, int army);
int ExperienceToRank(int exp);

int Co_GetRankFiltered(int coId, int army)
{
    return ExperienceToRank(Co_GetExperienceFiltered(coId, army));
}
