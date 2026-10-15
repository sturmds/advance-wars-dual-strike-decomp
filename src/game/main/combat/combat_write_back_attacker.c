#include "types.h"
#include "game/Unk_02080440.h"

void Combatant_WriteBack(Combatant* combatant, Combatant* other);

/* Writes the attacker's hp and ammo back to its unit, with no second
 * combatant. Nothing calls it yet. */
void Combat_WriteBackAttacker(void) {
  Combatant_WriteBack(&MAP_STATE->combatants[0], NULL);
}
