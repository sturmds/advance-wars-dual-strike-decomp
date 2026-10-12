#include "types.h"

s32 Rng_Range(s32 min, s32 max);
int Ov006_FindRandomCell(int* x, int* y, int cell);
void Ov006_SetMapCell(int x, int y, int cell);

/* Turns 20 to 40 random plain tiles (cell 1) into wood (cell 135). */
void Ov006_ScatterWoods(void) {
  int x;
  int y;
  int i;
  int count;

  count = Rng_Range(20, 40);
  for (i = 0; i < count; i++) {
    if (Ov006_FindRandomCell(&x, &y, 1) == 0) {
      return;
    }
    Ov006_SetMapCell(x, y, 135);
  }
}
