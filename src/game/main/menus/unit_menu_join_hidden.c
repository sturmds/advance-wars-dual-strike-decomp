#include "types.h"
#include "game/Unk_02080440.h"

BOOL Unit_CanJoin(MapUnit* unit, MapUnit* other);

/* Unit menu entry test: 1 hides the entry. Join is shown when a unit stands on
 * the destination tile and the acting unit can join it. The Explode, Capture
 * and Board tests hide their entries while this one shows. */
s8 UnitMenu_JoinHidden(void) {
  u8 slot;

  slot =
      MAP_STATE->mapTiles
          ->unitAt[MAP_STATE->actingUnitDestX +
                   MAP_STATE->mapTiles->rowStart[MAP_STATE->actingUnitDestY]];
  if (slot == 0) {
    return 1;
  }
  return !Unit_CanJoin(MAP_STATE->actingUnit, &MAP_STATE->units[slot]);
}
