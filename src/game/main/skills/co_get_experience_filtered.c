#include "types.h"

int Co_GetArmyExperience(int army, int coId);
extern u32 data_0229eaec;

int Co_GetExperienceFiltered(int coId, int army)
{
    /* Without data_0229eaec, every army reads the shared CO experience (army 0). */
    if (data_0229eaec == 0) {
        army = 0;
    }
    return Co_GetArmyExperience(army, coId & 0x7f);
}
