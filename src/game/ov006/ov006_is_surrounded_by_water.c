#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])
#define IS_WATER(tile) ((tile) == TERRAIN_REEF || (tile) == TERRAIN_SEA ? 1 : 0)

/* One mask bit per tile of the 3x3 square, bit 8 top-left to bit 0 bottom-right. */
#define ALL_NINE 0x1ff

/* 1 if the tile at x, y and its eight neighbours are all sea or reef, else
 * 0. A neighbour outside the map counts as land. */
int Ov006_IsSurroundedByWater(int x, int y) {
  u32 mask = 0;
  u8 tile;

  tile = TILE_AT(x, y);
  mask |= IS_WATER(tile) << 4;
  if (mask == 0) {
    return 0;
  }
  if (y > 0) {
    if (x > 0) {
      tile = TILE_AT(x - 1, y - 1);
      mask |= IS_WATER(tile) << 8;
    }
    tile = TILE_AT(x, y - 1);
    mask |= IS_WATER(tile) << 7;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      tile = TILE_AT(x + 1, y - 1);
      mask |= IS_WATER(tile) << 6;
    }
  }
  if (x > 0) {
    tile = TILE_AT(x - 1, y);
    mask |= IS_WATER(tile) << 5;
  }
  if (x < STATE->unk_1c4->mapWidth - 1) {
    tile = TILE_AT(x + 1, y);
    mask |= IS_WATER(tile) << 3;
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      tile = TILE_AT(x - 1, y + 1);
      mask |= IS_WATER(tile) << 2;
    }
    tile = TILE_AT(x, y + 1);
    mask |= IS_WATER(tile) << 1;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      tile = TILE_AT(x + 1, y + 1);
      mask |= IS_WATER(tile);
    }
  }
  return mask == ALL_NINE;
}
