#include "types.h"

int Survival_GetMapCount(int mode);
int Survival_GetMapId(int mode, int index);
s32 HwMod32(s32 num, s32 den);

/* Record slot of the map at position index of a Survival mode's list.
 * Modes 0-2 use the position itself. Modes 3-5 share the records of modes
 * 0-2: a map that is also in that list uses its position there; any other
 * map uses its own position plus the length of that list. */
int Survival_GetRecordIndex(int mode, int index) {
  /* needed to match: declaration order */
  int i;
  int j;
  int base;
  int k;

  index = HwMod32(index, Survival_GetMapCount(mode));
  switch (mode) {
    case 0:
    case 1:
    case 2:
      return index;
    case 3:
      base = 0;
      break;
    case 4:
      base = 1;
      break;
    case 5:
      base = 2;
      break;
    default:
      return 0;
  }
  for (i = 0; i < Survival_GetMapCount(base); i++) {
    if (Survival_GetMapId(mode, index) == Survival_GetMapId(base, i)) {
      return i;
    }
  }
  /* The inner search has no effect: the result is always index plus the
     * length of the base list. */
  for (j = 0; j < index; j++) {
    for (k = 0; k < Survival_GetMapCount(base); k++) {
      if (Survival_GetMapId(mode, j) == Survival_GetMapId(base, k)) {
        break;
      }
    }
  }
  return j + Survival_GetMapCount(base);
}
