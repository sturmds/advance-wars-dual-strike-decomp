#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int UnitType_GetAttackKind(u8 unitType);

/* TRUE when the unit has no weapon and carries nothing. */
BOOL Unit_IsHarmless(MapUnit* unit) {
  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
    if (unit->cargo[0] == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
