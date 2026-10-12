#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Writes value into the three range map tiles x to x + 2 of every row from
 * startRow to the bottom of the map. */
void RangeMap_FillColumnsBelow(int x, u16 startRow, s8 value) {
  u16 row;
  int i;

  for (row = startRow; row < STATE->unk_1c4->mapHeight; row++) {
    for (i = 0; i < 3; i++) {
      STATE->rangeMap[row][x + i] = value;
    }
  }
}
