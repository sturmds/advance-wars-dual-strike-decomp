#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Sets every map tile of a vision grid (game/vision.h) to value: 1 lights
 * the map, 0 darkens it for fog. Pipe and pipe seam tiles are always lit. */
void VisionGrid_Reset(s8* grid, int value) {
  int x;
  int y;
  int tile;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      grid[x + MAP_STATE->mapTiles->rowStart[y]] = (s8)value;
      tile = x + MAP_STATE->mapTiles->rowStart[y];
      /* needed to match: a switch, not || */
      switch (MAP_STATE->mapTiles->tiles[tile]) {
        case TERRAIN_PIPE:
        case TERRAIN_PIPE_SEAM:
          grid[tile] = 1;
          break;
      }
    }
  }
}
