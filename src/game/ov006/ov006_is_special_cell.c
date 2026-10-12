#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 if the map cell code at (x, y) is one of 252, 253, 284 and 285. */
int Ov006_IsSpecialCell(int x, int y) {
  u16 cell = STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]];

  return cell == 252 || cell == 253 || cell == 284 || cell == 285;
}
