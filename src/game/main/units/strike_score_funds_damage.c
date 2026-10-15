#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Returns how the current player's unitType attacks, from its range after
 * CO bonuses: 0 direct, 1 indirect, 2 no weapon, 3 Black Bomb, 4 Oozium. */
int UnitType_GetAttackKind(u8 unitType);

/* Target score for the CO power strikes, like StrikeScore_HpDamage but
 * weighted by unit cost: the funds value the strike destroys, in units of
 * costDiv10 per hp point. */
int StrikeScore_FundsDamage(MapUnit* unit, int damage, int isEnemy) {
  u16 hpLost;
  u32 score;

  if (unit->type == UNIT_OOZIUM) {
    return 0;
  }
  if (unit->hp <= 10) {
    return 2;
  }
  if (UnitType_GetAttackKind(unit->type) == 2 && isEnemy == 1) {
    return 2;
  }

  score = Ov000_gUnitStats[unit->type].costDiv10;
  if (unit->hp > damage) {
    hpLost = damage;
  } else {
    hpLost = unit->hp - 1;
  }
  score *= hpLost;
  if (UnitType_GetAttackKind(unit->type) == 2 && isEnemy == 0) {
    score /= 2;
  }
  return score;
}
