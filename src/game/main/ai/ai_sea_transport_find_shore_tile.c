#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) (MAP->tiles[(x) + MAP->rowStart[y]])

/* Writes the x and y of the first Shoal or Port tile (rows from the top) that
 * the range map reaches to out and returns TRUE; FALSE when there is none. */
BOOL AiSeaTransport_FindShoreTile(s16* out) {
  int x;
  int y;
  int width;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    width = STATE->unk_1c4->mapWidth;
    for (x = 0; x < width; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_SHOAL ||
          MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_PORT) {
        out[0] = x;
        out[1] = y;
        return TRUE;
      }
    }
  }
  return FALSE;
}
