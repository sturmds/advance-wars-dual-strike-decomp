#include "types.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when the unit type's move type has a positive cost on the terrain id. */
BOOL UnitType_CanEnterTerrain(int unitType, u8 terrain) {
  return CHART[Ov000_gUnitStats[unitType].moveType][terrain & 31] > 0;
}
