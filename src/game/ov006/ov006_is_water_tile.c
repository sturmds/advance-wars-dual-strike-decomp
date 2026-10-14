#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_IsSpecialCell(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 for an unowned sea, shoal or reef tile at (x, y), and for a river tile
 * when Ov006_IsSpecialCell is nonzero; else 0. */
int Ov006_IsWaterTile(int x, int y) {
  int result = 0;

  switch (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]]) {
    case TERRAIN_SEA:
    case TERRAIN_SHOAL:
    case TERRAIN_REEF:
      result = 1;
      break;
    case TERRAIN_RIVER:
      result = Ov006_IsSpecialCell(x, y);
      break;
  }
  return result;
}
