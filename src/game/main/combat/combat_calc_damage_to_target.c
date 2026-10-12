#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int TargetKind_GetChartColumn(int target); /* damage chart column of target */
int Unit_GetChartDamage(MapUnit* unit, int targetType, int weapon, int arg3);
int UnitType_GetChartDamage(
    int unitType, int column,
    int weapon); /* base damage in percent; 0: cannot hit */
BOOL Mission_HasPendingEvent(int target); /* checks the map id */
void Combatant_Init(Combatant* combatant, s16 unitIndex);
void Combatant_CalcDamage(Combatant* combatant, int isCounter, int rollLuck);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Hp as the game shows it: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp == 0 ? 0 : HwDiv32((unit)->hp - 1, 10) + 1)

/* Damage a unit would deal to a target kind (0-13, not a unit slot; probably
 * map objects), from pos (x, y) or from where it stands when pos is NULL.
 * Fills combatants[0]: the primary weapon when the unit has ammo, else the
 * secondary weapon if it can hit that target. No luck; the damage scales
 * with shown hp. Some maps cut any damage to 1. The unit's position is put
 * back after. */
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos) {
  int primaryDamage;
  int secondaryDamage;
  int canUseSecondary;
  MapUnit* unit;
  int x;
  int y;

  primaryDamage = 0;
  unit = &STATE->units[unitIndex];
  x = unit->x;
  y = unit->y;
  secondaryDamage = primaryDamage;
  canUseSecondary = primaryDamage;
  if (pos != NULL) {
    unit->x = pos[0];
    unit->y = pos[1];
  }
  Combatant_Init(&STATE->combatants[0], unitIndex);
  if (unit->ammo != 0) {
    primaryDamage =
        Unit_GetChartDamage(unit, TargetKind_GetChartColumn(target), 0, 0);
  } else {
    canUseSecondary = UnitType_GetChartDamage(
        unit->type, TargetKind_GetChartColumn(target), 1);
    if (canUseSecondary) {
      secondaryDamage =
          Unit_GetChartDamage(unit, TargetKind_GetChartColumn(target), 1, 0);
    }
  }
  if (primaryDamage <= secondaryDamage) {
    if (canUseSecondary) {
      STATE->combatants[0].weapon = COMBAT_WEAPON_SECONDARY;
      STATE->combatants[0].baseDamage = secondaryDamage;
    }
  } else {
    STATE->combatants[0].weapon = COMBAT_WEAPON_PRIMARY;
    STATE->combatants[0].baseDamage = primaryDamage;
    STATE->combatants[0].ammo--;
  }
  Combatant_CalcDamage(&STATE->combatants[0], 0, 0);
  STATE->combatants[0].damage = HwDiv32(
      STATE->combatants[0].damageNoLuck * SHOWN_HP(STATE->combatants[0].unit),
      10);
  STATE->combatants[0].damageNoLuck = STATE->combatants[0].damage;
  if (Mission_HasPendingEvent(target) && STATE->combatants[0].damage > 0) {
    STATE->combatants[0].damage = 1;
    STATE->combatants[0].damageNoLuck = STATE->combatants[0].damage;
  }
  unit->x = x;
  unit->y = y;
}
