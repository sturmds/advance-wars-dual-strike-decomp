#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* FALSE when player 3 owns any of the tiles (1, 4), (16, 1), (4, 16) or (19, 13). Which map this is for is unknown. */
BOOL func_ov001_02351b88(void) {
  if (TILE_OWNER(1, 4) == 3) {
    return FALSE;
  }
  if (TILE_OWNER(16, 1) == 3) {
    return FALSE;
  }
  if (TILE_OWNER(4, 16) == 3) {
    return FALSE;
  }
  return TILE_OWNER(19, 13) != 3;
}
