#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when tile (x, y) holds a unit (MapTiles.unk_508) whose owner is on
 * the same team as the given player. */
BOOL Tile_HasTeammateUnit(u8 player, int x, int y) {
  u8 slot = STATE->mapTiles->unk_508[x + STATE->mapTiles->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return STATE->players[player].team == STATE->players[(slot >> 6) + 1].team;
}
