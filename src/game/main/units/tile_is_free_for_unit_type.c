#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

/* Needed to match: the state array is indexed first. */
#define STATE STATE_IDX_FIRST
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])
/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when tile (x, y) is on the map, holds no unit and has terrain that
 * unitType's move type can enter. checkUnitAt picks the unit grid: TRUE
 * MapTiles.unitAt, FALSE MapTiles.unk_508. */
BOOL Tile_IsFreeForUnitType(s16 x, s16 y, int unitType, BOOL checkUnitAt) {
  if (x < 0 || y < 0 || x >= STATE->unk_1c4->mapWidth ||
      y >= STATE->unk_1c4->mapHeight) {
    return FALSE;
  }
  if (checkUnitAt) {
    if (STATE->mapTiles->unitAt[TILE_INDEX(x, y)] != 0) {
      return FALSE;
    }
  } else {
    if (STATE->mapTiles->unk_508[TILE_INDEX(x, y)] != 0) {
      return FALSE;
    }
  }
  if (CHART[Ov000_gUnitStats[unitType].moveType]
           [MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE_INDEX(x, y)])] < 0) {
    return FALSE;
  }
  return TRUE;
}
