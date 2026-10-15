#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

/* Writes value into the range map tiles x to x + 2 (a Deathray's width) of
 * every row from startRow to the bottom of the map: the Deathray's fire
 * range. */
void RangeMap_FillColumnsBelow(int x, u16 startRow, s8 value) {
  u16 row;
  int i;

  for (row = startRow; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (i = 0; i < DEATHRAY_SIZE; i++) {
      MAP_STATE->rangeMap[row][x + i] = value;
    }
  }
}
