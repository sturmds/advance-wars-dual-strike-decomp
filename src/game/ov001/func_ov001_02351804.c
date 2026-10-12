#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* TRUE when the tiles at (7, 7), (7, 12), (12, 7) and (12, 12) all belong to player 1. Which map this is for is unknown. */
BOOL func_ov001_02351804(void) {
  if (TILE_OWNER(7, 7) != 1) {
    return FALSE;
  }
  if (TILE_OWNER(7, 12) != 1) {
    return FALSE;
  }
  if (TILE_OWNER(12, 7) != 1) {
    return FALSE;
  }
  return TILE_OWNER(12, 12) == 1;
}
