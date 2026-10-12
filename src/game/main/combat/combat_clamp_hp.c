#include "types.h"
#include "game/combatant.h"

/* Stops hp going below 0. When both sides would be destroyed, the one whose
 * hp went lower is destroyed and the other survives with 1 hp; on a tie the
 * attacker survives. */
void Combat_ClampHp(Combatant* attacker, Combatant* defender) {
  if (attacker->hp <= 0 && defender->hp <= 0) {
    if (attacker->hp < defender->hp) {
      defender->hp = 1;
      attacker->hp = 0;
    } else {
      attacker->hp = 1;
      defender->hp = 0;
    }
    return;
  }
  if (attacker->hp < 0) {
    attacker->hp = 0;
  }
  if (defender->hp < 0) {
    defender->hp = 0;
  }
}
