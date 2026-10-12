#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Combatant_WriteBack(Combatant* combatant, Combatant* other);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Writes the attacker's hp and ammo back to its unit, with no second
 * combatant. Nothing calls it yet. */
void Combat_WriteBackAttacker(void) {
  Combatant_WriteBack(&STATE->combatants[0], NULL);
}
