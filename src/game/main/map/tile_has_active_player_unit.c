#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAP (GAME->mapTiles)

/* TRUE when tile (x, y) holds a unit (MapTiles.unk_508) whose owner has
 * Player.visionGrids bit 1 set. A slot's owner is slot / 64 + 1. */
BOOL Tile_HasActivePlayerUnit(int x, int y) {
  int slot = MAP->unk_508[x + MAP->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return (GAME->players[(slot >> 6) + 1].visionGrids & 2) != 0;
}
