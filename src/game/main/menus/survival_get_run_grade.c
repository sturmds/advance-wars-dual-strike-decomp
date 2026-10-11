#include "types.h"
#include "game/Unk_022a7b80.h"

int Survival_GetGrade(int mode, u32 value);

extern Unk_022a7b80 data_022a7b80;

/* Grade of the current run. Modes 0 to 2 grade the remaining budget
 * (unk_08), the others the map count (unk_06). */
int Survival_GetRunGrade(void)
{
    switch (data_022a7b80.unk_07) {
    case 0:
    case 1:
    case 2:
        return Survival_GetGrade(data_022a7b80.unk_07, data_022a7b80.unk_08);
    default:
        return Survival_GetGrade(data_022a7b80.unk_07, data_022a7b80.unk_06);
    }
}
