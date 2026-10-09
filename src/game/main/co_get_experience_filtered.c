#include "types.h"

int func_02081aa4(int army, int coId);
extern u32 data_0229eaec;

int Co_GetExperienceFiltered(int coId, int army)
{
    /* Without data_0229eaec, every army reads the shared CO experience (army 0). */
    if (data_0229eaec == 0) {
        army = 0;
    }
    return func_02081aa4(army, coId & 0x7f);
}
