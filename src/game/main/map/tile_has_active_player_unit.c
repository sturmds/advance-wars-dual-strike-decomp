#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when tile (x, y) holds a unit (MapTiles.unk_508) whose owner has
 * Player.visionGrids bit 1 set. A slot's owner is slot / 64 + 1. */
BOOL Tile_HasActivePlayerUnit(int x, int y) {
  int slot = MAP_TILES->unk_508[x + MAP_TILES->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return (MAP_STATE->players[(slot >> 6) + 1].visionGrids & 2) != 0;
}
