#include "types.h"
#include "game/combatant.h"

void func_020edb84(MapUnit* unit);

/* Writes the combatant's hp and ammo back to its unit. A unit left with 0
 * hp goes to func_020edb84. other is not read. */
void Combatant_WriteBack(Combatant* combatant, Combatant* other) {
  combatant->unit->hp = combatant->hp;
  combatant->unit->ammo = combatant->ammo;
  if (combatant->unit->hp == 0) {
    func_020edb84(combatant->unit);
  }
}
