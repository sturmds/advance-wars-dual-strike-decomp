#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

/* Default movement cost chart of the first CO record. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when a Lander's move type has a movement cost over 0 on the terrain
 * of the given map tile. */
BOOL Lander_CanEnterTile(int tile) {
  return CHART[Ov000_gUnitStats[UNIT_LANDER].moveType][MAP_TILE_TERRAIN(tile)] >
         0;
}
