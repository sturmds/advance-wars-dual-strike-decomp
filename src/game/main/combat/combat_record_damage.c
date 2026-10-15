#include "types.h"
#include "game/combatant.h"
#include "game/math.h"

void Unit_AddDamageTaken(MapUnit* unit, int hpLost);
void DamageToFunds(MapUnit* attacker, MapUnit* target, int hpLost);

/* Hp shown on screen: 1-10, or 0 for 0 hp. */
#define SHOWN_HP(hp) ((hp) == 0 ? 0 : HwDiv32((hp) - 1, 10) + 1)

/* Records the shown hp the battle took from each side. The combatant's
 * own loss goes to its owner's damage taken. With other given, the other
 * side's loss pays out the combatant's player (DamageToFunds). */
void Combat_RecordDamage(Combatant* combatant, Combatant* other) {
  int shownAfter = SHOWN_HP(combatant->hp);
  int shownBefore = SHOWN_HP(combatant->unit->hp);

  Unit_AddDamageTaken(combatant->unit, shownBefore - shownAfter);
  if (other == NULL) {
    return;
  }
  shownAfter = SHOWN_HP(other->hp);
  shownBefore = SHOWN_HP(other->unit->hp);
  DamageToFunds(combatant->unit, other->unit, shownBefore - shownAfter);
}
