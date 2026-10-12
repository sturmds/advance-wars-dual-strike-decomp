#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);
int UnitType_GetChartDamage(int unitType, int column, int weapon);
int Unit_GetChartDamage(MapUnit* unit, int targetType, int weapon,
                        BOOL targetDived);
int DamageChart_GetColumn(int targetType,
                          BOOL targetDived); /* damage chart column */
int Unit_GetDefense(MapUnit* unit,
                    int attackerType); /* percent: 100 plus bonuses */

/* Sets attacker->defense, then picks the weapon attacker uses on defender
 * and sets baseDamage from the damage chart. Next to it (distance 1) the
 * secondary weapon competes with the primary weapon of a direct unit;
 * further away only the primary weapon fires, within range and only on
 * the attack (isAttack 1), not on a counter. The primary weapon is used only
 * when it deals more damage, and then uses one ammo. */
void Combatant_PickWeapon(Combatant* attacker, Combatant* defender,
                          s16 distance, int isAttack) {
  u16 player =
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], attacker->unit);
  u16 primaryDamage = 0;
  u16 secondaryDamage = primaryDamage;
  s16 canUseSecondary = primaryDamage;
  UnitStats* stats;

  /* needed to match: stats declared last and set here */
  stats = &Ov000_gUnitStats[attacker->unit->type];
  attacker->defense = Unit_GetDefense(attacker->unit, defender->unit->type);
  if (distance == 1) {
    canUseSecondary = UnitType_GetChartDamage(
        attacker->unit->type,
        DamageChart_GetColumn(defender->unit->type,
                              defender->unit->flags & MAP_UNIT_DIVED),
        1);
    if (canUseSecondary) {
      secondaryDamage =
          Unit_GetChartDamage(attacker->unit, defender->unit->type, 1,
                              defender->unit->flags & MAP_UNIT_DIVED);
    }
    if (stats->minRange == ATTACK_DIRECT && attacker->unit->ammo != 0) {
      primaryDamage =
          Unit_GetChartDamage(attacker->unit, defender->unit->type, 0,
                              defender->unit->flags & MAP_UNIT_DIVED);
    }
  } else if (stats->minRange <= distance &&
             distance <= GetUnitMaxRange(player, attacker->unit->type) &&
             attacker->unit->ammo != 0 && isAttack == 1) {
    primaryDamage = Unit_GetChartDamage(attacker->unit, defender->unit->type, 0,
                                        defender->unit->flags & MAP_UNIT_DIVED);
  }
  if (primaryDamage <= secondaryDamage) {
    if (canUseSecondary) {
      attacker->weapon = COMBAT_WEAPON_SECONDARY;
      attacker->baseDamage = secondaryDamage;
    }
  } else {
    attacker->weapon = COMBAT_WEAPON_PRIMARY;
    attacker->baseDamage = primaryDamage;
    attacker->ammo--;
  }
}
