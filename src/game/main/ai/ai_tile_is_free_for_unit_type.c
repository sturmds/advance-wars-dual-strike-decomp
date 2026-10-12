/* Needed to match: with dead-assignment removal on, mwcc swaps the operands
 * of the add that indexes the state slots. */
#pragma opt_dead_assignments off

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when no unit stands on (x, y) and units of this type can move onto
 * its terrain (default movement chart). */
BOOL AiTile_IsFreeForUnitType(int x, int y, int unitType) {
  int index;
  Unk_02080440_48** slot = &data_027c027c->unk_48[data_027c0284];
  MapTiles* map = (*slot)->mapTiles;

  index = x + map->rowStart[y];

  if (map->unitAt[index] != 0) {
    return FALSE;
  }
  return CHART[Ov000_gUnitStats[unitType].moveType]
              [MAP_TILE_TERRAIN(map->tiles[index])] >= 0;
}
