#include "types.h"
#include "game/design_map_gen.h"

/* count: as many elements as fit before the next symbol */
s8 Ov006_gGenMarks[0x14][30];

s32 Rng_Range(s32 min, s32 max);
int Ov006_FindRandomCell(int* x, int* y, int cell);
void Ov006_SetMapCell(int x, int y, int cell);
void Ov006_GrowCells(int a, int b);
void Ov006_SettleSeedTiles(void);
void Ov006_MarkMapBorder(void);

/* Turns 8 to 12 random plain tiles (cell 1) into mountain (cell 32) and marks each
 * with 2 in Ov006_gGenMarks, a grid of 30-byte rows. */
void Ov006_ScatterMountains(void) {
  int x;
  int y;
  int i;
  int count;

  Ov006_MarkMapBorder();
  count = Rng_Range(8, 12);
  for (i = 0; i < count; i++) {
    if (Ov006_FindRandomCell(&x, &y, 1) == 0) {
      break;
    }
    Ov006_SetMapCell(x, y, 32);
    Ov006_gGenMarks[y][x] = 2;
  }
  Ov006_SettleSeedTiles();
  Ov006_GrowCells(32, 1);
}
