#include "types.h"
#include "game/Unk_02080440.h"

BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, s16 x, s16 y);

/* Unit_IsVisibleToActivePlayers for the unit in a slot of the units array,
 * at its own tile. */
BOOL UnitSlot_IsVisible(int slot) {
  MapUnit* unit = &MAP_STATE->units[slot];

  return Unit_IsVisibleToActivePlayers(unit, unit->x, unit->y);
}
