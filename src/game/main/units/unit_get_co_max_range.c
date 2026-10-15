#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

int UnitType_GetMaxRange(int unitType);
int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);

/* Maximum firing range of a unit on the map: the type's range plus the
 * range bonus of its owner's front CO, with no power on. The owner comes from
 * the unit's slot: 64 slots per player (MAP_UNITS_PER_PLAYER),
 * written as a shift to match. */
int Unit_GetCoMaxRange(MapUnit* unit) {
  int baseRange = UnitType_GetMaxRange(unit->type);

  return baseRange + GetCoRangeBonus(((unit - MAP_STATE->units) >> 6) + 1, 0, 0,
                                     unit->type);
}
