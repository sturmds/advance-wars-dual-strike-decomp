#include "types.h"
#include "game/Unk_022a7b80.h"

int Survival_GetGrade(int mode, u32 value);

extern Unk_022a7b80 data_022a7b80;

/* Grade of the current run. Modes 0 to 2 grade the remaining budget
 * (budgetLeft), the others the map count (mapIndex). */
int Survival_GetRunGrade(void) {
  switch (data_022a7b80.mode) {
    case 0:
    case 1:
    case 2:
      return Survival_GetGrade(data_022a7b80.mode, data_022a7b80.budgetLeft);
    default:
      return Survival_GetGrade(data_022a7b80.mode, data_022a7b80.mapIndex);
  }
}
