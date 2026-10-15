#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

s8 Unit_GetDropDirections(int x, int y, int slot);

#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when the carried unit in cargo slot cargoIndex has a free tile to
 * drop onto next to (x, y) and its move type can enter the terrain at
 * (x, y) itself. */
BOOL Transport_CanUnloadCargo(MapUnit* carrier, int x, int y, int cargoIndex) {
  u8* slot = &carrier->cargo[cargoIndex];

  if (Unit_GetDropDirections(x, y, *slot) == 0) {
    return FALSE;
  }
  return CHART[Ov000_gUnitStats[MAP_STATE->units[*slot].type].moveType]
              [MAP_TILE_TERRAIN(
                  MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)])] != -1;
}
