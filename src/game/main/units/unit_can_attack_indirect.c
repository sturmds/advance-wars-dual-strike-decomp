#include "types.h"
#include "game/Unk_02080440.h"

int GetUnitMaxRange(int player, int unitType);

/* TRUE when the unit has a ranged weapon (maximum range above 1 for its
 * owner's CO) and ammo left. */
BOOL Unit_CanAttackIndirect(MapUnit* unit) {
  if (GetUnitMaxRange(MAP_UNIT_OWNER(MAP_STATE, unit), unit->type) == 1) {
    return FALSE;
  }
  return unit->ammo != 0;
}
