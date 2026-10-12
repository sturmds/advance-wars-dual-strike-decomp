#include "types.h"
#include "game/ov003_unit.h"

#define STATE data_ov003_02364928

/* How far a unit looks for a target: 128, or 0 (no targets) in the modes
 * other than 1 and 2 when unk_1df_5 is clear. Which mode is which is not known. */
int Ov003_GetTargetRange(void) {
  if (STATE->unk_1df_5) {
    return 128;
  }
  switch (STATE->unk_1df_2) {
    case 1:
    case 2:
      return 128;
  }
  return 0;
}
