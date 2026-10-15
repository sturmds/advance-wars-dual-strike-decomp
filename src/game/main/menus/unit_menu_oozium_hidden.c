#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* Unit menu entry test (table at 0x02166030): 1 hides the entry. Shown only
 * for an Oozium whose destination tile holds a unit of another team. The
 * entry's action is func_020bcfe0. */
BOOL UnitMenu_OoziumHidden(void) {
  u8 slot;

  if (MAP_STATE->actingUnit->type != UNIT_OOZIUM) {
    return TRUE;
  }
  slot =
      MAP_STATE->mapTiles
          ->unitAt[MAP_STATE->actingUnitDestX +
                   MAP_STATE->mapTiles->rowStart[MAP_STATE->actingUnitDestY]];
  if (slot == 0 ||
      MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, MAP_STATE->actingUnit)]
              .team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, &MAP_STATE->units[slot])]
              .team) {
    return TRUE;
  }
  return FALSE;
}
