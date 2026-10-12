#include "types.h"

u8 Ov005_GetCoListEntryRow(int index);
u8 Ov005_GetCoListEntryColumn(int index);

/* Table index of the entry whose bytes 2 and 3 are x and y (guessed as grid
 * position), or -1. */
int Ov005_FindCoListEntryAt(int x, int y) {
  int i;

  for (i = 0; i < 33; i++) {
    if (x == Ov005_GetCoListEntryColumn(i) && y == Ov005_GetCoListEntryRow(i)) {
      return i;
    }
  }
  return -1;
}
