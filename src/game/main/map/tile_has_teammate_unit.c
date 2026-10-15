#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when tile (x, y) holds a unit (MapTiles.unk_508) whose owner is on
 * the same team as the given player. */
BOOL Tile_HasTeammateUnit(u8 player, int x, int y) {
  u8 slot = MAP_STATE->mapTiles->unk_508[x + MAP_STATE->mapTiles->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return MAP_STATE->players[player].team ==
         MAP_STATE->players[(slot >> 6) + 1].team;
}
