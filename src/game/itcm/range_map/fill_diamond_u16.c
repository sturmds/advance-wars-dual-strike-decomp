#include "types.h"
#include "game/Unk_02080440.h"

/* Writes value into every range map tile within radius steps of (x, y),
 * skipping tiles off the map: row y and the rows below it first, then the
 * rows above. */
void RangeMap_FillDiamondU16(u16 x, u16 y, u16 radius, s8 value) {
  int row;
  int halfWidth;
  int col;
  int colEnd;
  int count;

  halfWidth = radius;
  for (row = y; row <= y + radius; row++, halfWidth--) {
    if (row >= MAP_STATE->unk_1c4->mapHeight) {
      break;
    }
    col = x - halfWidth;
    count = halfWidth * 2 + 1;
    if (col < 0) {
      count += col;
      col = 0;
    }
    colEnd = col + count;
    if (colEnd > MAP_STATE->unk_1c4->mapWidth) {
      colEnd = MAP_STATE->unk_1c4->mapWidth;
    }
    for (; col < colEnd; col++) {
      MAP_STATE->rangeMap[row][col] = value;
    }
  }
  halfWidth = radius - 1;
  for (row = y - 1; row >= y - radius; row--, halfWidth--) {
    if (row < 0) {
      return;
    }
    col = x - halfWidth;
    count = halfWidth * 2 + 1;
    if (col < 0) {
      count += col;
      col = 0;
    }
    colEnd = col + count;
    if (colEnd > MAP_STATE->unk_1c4->mapWidth) {
      colEnd = MAP_STATE->unk_1c4->mapWidth;
    }
    for (; col < colEnd; col++) {
      MAP_STATE->rangeMap[row][col] = value;
    }
  }
}
