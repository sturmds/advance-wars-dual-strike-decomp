#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 Ov006_gWaterCellTable[];

int Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: 1 when tile x, y is water (sea, shoal, reef, ...) */
s16 Ov006_GetShoalGraphic(int x, int y);
BOOL Ov006_IsBridgeTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->unk_1c4)

/* Picks the cell that tile x, y gets from the water around it: each of the
 * nine tiles of the 3x3 block that Ov006_IsLandNeighbor counts sets one bit
 * (0x100 up-left through 1 down-right, row by row), and the 512-entry table
 * gives the cell. Shoal uses Ov006_GetShoalGraphic instead; rivers and bridge
 * pieces get -1 (keep the cell). */
s16 Ov006_GetWaterCell(int x, int y) {
  int mask = 0;
  int terrain;
  int cell;

  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y - 1) << 8;
    }
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 7;
    if (x < MAP->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 5;
  }
  mask |= Ov006_IsLandNeighbor(x, y) << 4;
  if (x < MAP->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 3;
  }
  if (y < MAP->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsLandNeighbor(x, y + 1) << 1;
    if (x < MAP->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y + 1);
    }
  }

  terrain = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];
  if (terrain == TERRAIN_SHOAL) {
    cell = Ov006_GetShoalGraphic(x, y);
  } else if (terrain == TERRAIN_RIVER) {
    cell = -1;
  } else if (terrain == TERRAIN_BRIDGE) {
    if (Ov006_IsBridgeTile(x, y)) {
      cell = -1;
    } else {
      cell = Ov006_gWaterCellTable[mask];
    }
  } else {
    cell = Ov006_gWaterCellTable[mask];
  }
  return cell;
}
