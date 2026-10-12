#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Player_GetCounterBonus(
    int playerIndex); /* front CO's CoPowerBlock.counterBonus */

/* Counter damage bonus, in percent, of the player that owns the unit. */
int Combatant_GetCounterBonus(Combatant* combatant) {
  return Player_GetCounterBonus(
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], combatant->unit));
}
