#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/terrain.h"

void RangeMap_FillDiamondU16(u16 x, u16 y, u16 radius, s8 value);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

#define CRYSTAL_RANGE 2
#define OBELISK_RANGE 4
#define TILE_HP_FLAG 0x80 /* see Unit_IsBlackHoleOnFlaggedTile */
#define TILE_HP_VALUE_MASK 0x7f

/* Sets bit 7 of tileHp on every tile within 2 of an unowned Black Crystal or
 * within 4 of a Black Obelisk, and clears it elsewhere. The two loops use
 * separate counters: needed to match. */
void Map_MarkCrystalObeliskRange(void) {
  int x;
  int y;
  int col;
  int row;

  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(0);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      switch (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]]) {
        case TERRAIN_BLACK_CRYSTAL:
          RangeMap_FillDiamondU16(x, y, CRYSTAL_RANGE, 1);
          break;
        case TERRAIN_BLACK_OBELISK:
          RangeMap_FillDiamondU16(x, y, OBELISK_RANGE, 1);
          break;
      }
    }
  }
  for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
      if (STATE->rangeMap[row][col] != 0) {
        STATE->tileHp[col + STATE->mapTiles->rowStart[row]] |= TILE_HP_FLAG;
      } else {
        STATE->tileHp[col + STATE->mapTiles->rowStart[row]] &=
            TILE_HP_VALUE_MASK;
      }
    }
  }
}
