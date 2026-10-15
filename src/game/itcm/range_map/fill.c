#include "types.h"
#include "game/Unk_02080440.h"

/* Sets every tile rangeMap points to. s8 value matches the one-byte store. */
void RangeMap_Fill(s8 value) {
  int col;
  int row;

  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
      MAP_STATE->rangeMap[row][col] = value;
    }
  }
}
