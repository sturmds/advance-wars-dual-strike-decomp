#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int UnitType_GetAttackKind(int unitType);

/* Black Boat repair priority of a unit: 0 for a unit with no weapon or a
 * Black Bomb, or at 91 HP or more; otherwise by HP: 71-90 gives 4, 51-70
 * gives 5, 31-50 gives 3, 11-30 gives 2 and 1-10 gives 1. */
int AiBlackBoat_GetRepairPriority(MapUnit* unit) {
  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
    return 0;
  }
  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_BLACK_BOMB) {
    return 0;
  }
  if (unit->hp >= 91) {
    return 0;
  }
  if (unit->hp >= 71) {
    return 4;
  }
  if (unit->hp >= 51) {
    return 5;
  }
  if (unit->hp >= 31) {
    return 3;
  }
  if (unit->hp >= 11) {
    return 2;
  }
  return 1;
}
