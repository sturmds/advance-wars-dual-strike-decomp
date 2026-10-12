#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Returns how the current player's unitType attacks, from its range after
 * CO bonuses: 0 direct, 1 indirect, 2 no weapon, 3 Black Bomb, 4 Oozium. */
int UnitType_GetAttackKind(int unitType);

/* Target score for the CO power strikes, like StrikeScore_InfantryDamage
 * without the infantry and capture weights: the HP the strike takes. */
int StrikeScore_HpDamage(MapUnit* unit, int damage, int isEnemy) {
  u16 score;

  if (unit->type == UNIT_OOZIUM) {
    return 0;
  }
  if (unit->hp <= 10) {
    return 1;
  }
  if (UnitType_GetAttackKind(unit->type) == 2 && isEnemy == 1) {
    return 1;
  }

  if (unit->hp > damage) {
    score = damage;
  } else {
    score = unit->hp - 1;
  }
  if (UnitType_GetAttackKind(unit->type) == 2 && isEnemy == 0) {
    score /= 2;
  }
  return score;
}
