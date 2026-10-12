#include "types.h"
#include "game/battle_scene.h"

u32 HwDiv32(u32 num, u32 den);

/* HP as shown on screen: 1 to 10, rounded up. */
#define SHOWN_HP(hp) ((hp) == 0 ? 0 : HwDiv32((hp) - 1, 10) + 1)

#define SIDE(s) (Ov002_gBattleScene->sides[s])

/* Mood of a side's CO portrait after the fight: 0 normal, 1 happy (the other
 * side was destroyed), 2 the face used with the angry quote (this side was
 * destroyed, or ends 4 or more shown HP behind). A side with no CO stays
 * normal. */
int Battle_GetSideMood(int side) {
  int other;
  u32 otherHp;

  if (SIDE(side).coId == 0) {
    return 0;
  }
  if (SIDE(side).hpAfter == 0) {
    return 2;
  }
  other = (side + 1) & 1;
  if (SIDE(other).hpAfter == 0) {
    return 1;
  }
  otherHp = SHOWN_HP(SIDE(other).hpAfter);
  return SHOWN_HP(SIDE(side).hpAfter) + 4 <= otherHp ? 2 : 0;
}
