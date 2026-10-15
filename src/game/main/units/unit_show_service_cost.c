#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

void ServiceCostPopup_Start(s16* pos, int player, int amount, u32 chargeFunds,
                            int zoomVariant, int visible);

/* Gets the cost of a service to the unit from getCost (DailySupply_Step passes
 * Unit_ResupplyUnpaid or Unit_ResupplyCargo) and passes it, with the unit's tile
 * and the tile's MapTiles.unk_2338 byte, to ServiceCostPopup_Start. Returns FALSE
 * when the cost is 0. */
BOOL Unit_ShowServiceCost(MapUnit* unit, int (*getCost)(MapUnit* unit)) {
  int cost = getCost(unit);
  s16 pos[2];

  if (cost == 0) {
    return FALSE;
  }
  pos[0] = unit->x;
  pos[1] = unit->y;
  ServiceCostPopup_Start(
      pos, (u8)MAP_STATE->currentPlayer, cost, 0, 0,
      MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(unit->x, unit->y)]);
  return TRUE;
}
