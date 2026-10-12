#include "types.h"
#include "game/combatant.h"

s32 HwDiv32(s32 numerator, s32 denominator);

/* Applies attacker's hit to target. Both damage values scale by
 * (200 - target defense) percent; damageNoLuck, the number shown before
 * the battle, also scales by the attacker's shown hp (1-10) and stops at
 * 999. target->hp becomes the hp left. */
void Combatant_TakeHit(Combatant* target, Combatant* attacker) {
  int shownHp;

  if (attacker->unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(attacker->unit->hp - 1, 10) + 1;
  }
  attacker->damageNoLuck = HwDiv32(attacker->damageNoLuck * shownHp, 10);
  attacker->damageNoLuck =
      HwDiv32((200 - target->defense) * attacker->damageNoLuck, 100);
  if (attacker->damageNoLuck >= 999) {
    attacker->damageNoLuck = 999;
  }
  target->hpLost = HwDiv32((200 - target->defense) * attacker->damage, 100);
  target->hp = target->unit->hp - target->hpLost;
}
