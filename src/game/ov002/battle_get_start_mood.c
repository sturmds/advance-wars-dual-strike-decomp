#include "types.h"
#include "game/battle_scene.h"

u32 HwDiv32(u32 num, u32 den);

/* HP as shown on screen: 1 to 10, rounded up. */
#define SHOWN_HP(hp) ((hp) == 0 ? 0 : HwDiv32((hp) - 1, 10) + 1)

#define SIDE(s) (Ov002_gBattleScene->sides[s])

/* Mood of a side's CO portrait before the fight: 0 normal, 2 the face used
 * with the angry quote. Shown HP is the HP in tens, rounded up. A side with
 * no CO stays normal. When both sides hit back, a side is in mood 2 if both
 * have 4 or less shown HP, or if it is 4 or more behind. A side that does
 * not hit back is in mood 2 unless it is at least 3 ahead. */
int Battle_GetStartMood(int side) {
  int other;
  u32 myHp;
  u32 otherHp;

  if (SIDE(side).coId == 0) {
    return 0;
  }
  if (SIDE(side).counters == 0 || SIDE((side + 1) & 1).counters == 0) {
    if (SIDE(side).counters != 0) {
      return 0;
    }
    other = (side + 1) & 1;
    otherHp = SHOWN_HP(SIDE(other).hpBefore);
    myHp = SHOWN_HP(SIDE(side).hpBefore);
    return myHp >= otherHp + 3 ? 0 : 2;
  }
  other = (side + 1) & 1;
  if (SHOWN_HP(SIDE(side).hpBefore) <= 4 &&
      SHOWN_HP(SIDE(other).hpBefore) <= 4) {
    return 2;
  }
  otherHp = SHOWN_HP(SIDE(other).hpBefore);
  return SHOWN_HP(SIDE(side).hpBefore) + 4 <= otherHp ? 2 : 0;
}
