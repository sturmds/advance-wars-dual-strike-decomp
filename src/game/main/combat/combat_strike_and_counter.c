#include "types.h"
#include "game/combatant.h"

s32 HwDiv32(s32 numerator, s32 denominator);
void Combatant_TakeHit(Combatant* target, Combatant* attacker);

/* first hits second, then second hits back with the hp it has left. Each
 * damage value scales by the striker's shown hp (1-10). */
void Combat_StrikeAndCounter(Combatant* first, Combatant* second) {
  int shownHp;
  int hp;

  if (first->unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(first->unit->hp - 1, 10) + 1;
  }
  first->damage = HwDiv32(first->damage * shownHp, 10);
  Combatant_TakeHit(second, first);

  hp = second->hp;
  if (hp < 0) {
    hp = 0;
  }
  if (hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(hp - 1, 10) + 1;
  }
  second->damage = HwDiv32(second->damage * shownHp, 10);
  Combatant_TakeHit(first, second);
}
