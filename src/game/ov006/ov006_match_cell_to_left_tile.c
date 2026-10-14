#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ov006_cell_forms.h"
#include "game/terrain.h"

s16 Ov006_gCellsOpen[2][49] = {
#include "data/ov006/Ov006_gCellsOpen.inc.c"
};

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns the form of map cell `cell` that fits tile (x, y), going by the
 * tile to its left; -1 when `cell` is in neither list. Guess: the second
 * form draws the shadow that tall tiles cast to their right. */
int Ov006_MatchCellToLeftTile(int x, int y, int cell) {
  s16* open = Ov006_gCellsOpen[0];
  s16* besideTall = Ov006_gCellsBesideTall;
  int i;

  if (x <= 0) {
    for (i = 0; i < CELL_PAIR_COUNT; i++) {
      if (cell == *besideTall) {
        return *open;
      }
      open++;
      besideTall++;
    }
    return -1;
  }

  switch (STATE->mapTiles->tiles[x - 1 + STATE->mapTiles->rowStart[y]]) {
    case TERRAIN_MOUNTAIN:
    case TERRAIN_WOOD:
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_COM_TOWER:
    case MAP_TILE(TERRAIN_CITY, 1):
    case MAP_TILE(TERRAIN_HQ, 1):
    case MAP_TILE(TERRAIN_AIRPORT, 1):
    case MAP_TILE(TERRAIN_PORT, 1):
    case MAP_TILE(TERRAIN_BASE, 1):
    case MAP_TILE(TERRAIN_COM_TOWER, 1):
    case MAP_TILE(TERRAIN_CITY, 2):
    case MAP_TILE(TERRAIN_HQ, 2):
    case MAP_TILE(TERRAIN_AIRPORT, 2):
    case MAP_TILE(TERRAIN_PORT, 2):
    case MAP_TILE(TERRAIN_BASE, 2):
    case MAP_TILE(TERRAIN_COM_TOWER, 2):
    case MAP_TILE(TERRAIN_CITY, 3):
    case MAP_TILE(TERRAIN_HQ, 3):
    case MAP_TILE(TERRAIN_AIRPORT, 3):
    case MAP_TILE(TERRAIN_PORT, 3):
    case MAP_TILE(TERRAIN_BASE, 3):
    case MAP_TILE(TERRAIN_COM_TOWER, 3):
    case MAP_TILE(TERRAIN_CITY, 4):
    case MAP_TILE(TERRAIN_HQ, 4):
    case MAP_TILE(TERRAIN_AIRPORT, 4):
    case MAP_TILE(TERRAIN_PORT, 4):
    case MAP_TILE(TERRAIN_BASE, 4):
    case MAP_TILE(TERRAIN_COM_TOWER, 4):
      for (i = 0; i < CELL_PAIR_COUNT; i++) {
        if (cell == *open) {
          return *besideTall;
        }
        open++;
        besideTall++;
      }
      break;
    default:
      for (i = 0; i < CELL_PAIR_COUNT; i++) {
        if (cell == *besideTall) {
          return *open;
        }
        open++;
        besideTall++;
      }
      break;
  }
  return -1;
}
