#include "types.h"
#include "game/Unk_02080440.h"

/* 1 if the map cell code at (x, y) is one of 252, 253, 284 and 285. */
int Ov006_IsSpecialCell(int x, int y) {
  u16 cell =
      MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]];

  return cell == 252 || cell == 253 || cell == 284 || cell == 285;
}
