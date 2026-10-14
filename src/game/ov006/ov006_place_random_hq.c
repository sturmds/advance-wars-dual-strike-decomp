#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 Rng_Range(s32 min, s32 max);
void Ov006_SetMapCell(int x, int y, int cell);
TilePoint Ov006_PlaceHq(int x, int y, int owner, int baseCount,
                        int airportCount);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_PLAIN 1

/* Picks a random 5x5 area inside the map with no owned tile, turns it all
 * into plain, and puts owner's HQ with its bases and airports in it
 * (Ov006_PlaceHq). Returns the HQ tile. */
TilePoint Ov006_PlaceRandomHq(int owner, int baseCount, int airportCount) {
  int x;
  int y;
  int i;
  int j;

pick:
  x = Rng_Range(0, STATE->unk_1c4->mapWidth - 1);
  y = Rng_Range(0, STATE->unk_1c4->mapHeight - 1);
  for (j = y - 2; j <= y + 2; j++) {
    if (j < 0 || j >= STATE->unk_1c4->mapHeight) {
      goto pick;
    }
    for (i = x - 2; i <= x + 2; i++) {
      if (i < 0 || i >= STATE->unk_1c4->mapWidth) {
        goto pick;
      }
      if (STATE->mapTiles->tiles[STATE->mapTiles->rowStart[j] + i] &
          0xe0) { /* owner bits: owned tile */
        goto pick;
      }
    }
  }
  for (j = y - 2; j <= y + 2; j++) {
    for (i = x - 2; i <= x + 2; i++) {
      Ov006_SetMapCell(i, j, CELL_PLAIN);
    }
  }
  return Ov006_PlaceHq(x, y, owner, baseCount, airportCount);
}
