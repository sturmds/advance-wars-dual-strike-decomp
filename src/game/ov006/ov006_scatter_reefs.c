#include "types.h"

s32 Rng_Range(s32 min, s32 max);
int Ov006_FindRandomClearCell(int* x, int* y, int cell);
void Ov006_SetMapCell(int x, int y, int cell);

/* Turns 8 to 15 random sea tiles (cell 42) with a clear area around them into reef
 * (cell 360). Stops at the first search that finds none. */
void Ov006_ScatterReefs(void) {
  int x;
  int y;
  int i;
  int count;

  count = Rng_Range(8, 15);
  for (i = 0; i < count; i++) {
    if (Ov006_FindRandomClearCell(&x, &y, 42) == 0) {
      return;
    }
    Ov006_SetMapCell(x, y, 360);
  }
}
