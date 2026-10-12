#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

u32 HwDiv32(u32 num, u32 den);
int HwMod32(int num, int denom);
int MainLoop_GetFrameCount(void);

#define MENU ((MenuState*)&gMenuState)

/* HP as shown on screen: 1 to 10, rounded up. */
#define SHOWN_HP(hp) ((hp) != 0 ? HwDiv32((hp) - 1, 10) + 1 : 0)

/* Five 3-bit values, the first in the low bits. */
#define PACK(order)                                                         \
  ((order)[0] | ((order)[1] << 3) | ((order)[2] << 6) | ((order)[3] << 9) | \
   ((order)[4] << 12))

/* Picks four orders of the numbers 0-4 per side, each packed into 15 bits, shuffled by the frame
 * counter. unk_294: number 0 is not left first when the side shows 1 or 2 HP before or after.
 * unk_298: with 1 HP shown after, number 3 does not sit where unk_294 has 0. unk_29c: no rule.
 * unk_2a0: the slots in falling order of their unk_294 value. Needed to match: the last loop has
 * its own counter, and the order of the declarations. */
void Battle_ShuffleCellOrders(void) {
  int tmp;
  int order[5];
  int k;
  int i;
  int shift;
  int slot;
  int side;
  int j;
  u32 used;
  int best;

  for (side = 0; side < 2; side++) {
    for (i = 0; i < 5; i++) {
      order[i] = i;
    }
    for (i = 0; i < 5; i++) {
      j = HwMod32(MainLoop_GetFrameCount() >> (i + side), 5);
      tmp = order[i];
      order[i] = order[j];
      order[j] = tmp;
    }
    if (SHOWN_HP(Ov002_gBattleScene->sides[side].hpBefore) == 1 ||
        SHOWN_HP(Ov002_gBattleScene->sides[side].hpBefore) == 2 ||
        SHOWN_HP(Ov002_gBattleScene->sides[side].hpAfter) == 1 ||
        SHOWN_HP(Ov002_gBattleScene->sides[side].hpAfter) == 2) {
      for (i = 0; i < 5; i++) {
        if (order[i] == 0) {
          break;
        }
      }
      if (i == 0) {
        tmp = order[0];
        order[0] = order[(MainLoop_GetFrameCount() & 3) + 1];
        order[(MainLoop_GetFrameCount() & 3) + 1] = tmp;
      }
    }
    MENU->battle.unk_294[side] = PACK(order);

    for (i = 0; i < 5; i++) {
      order[i] = i;
    }
    for (i = 0; i < 5; i++) {
      j = HwMod32(MainLoop_GetFrameCount() >> (i + side + 1), 5);
      tmp = order[i];
      order[i] = order[j];
      order[j] = tmp;
    }
    if (SHOWN_HP(Ov002_gBattleScene->sides[side].hpAfter) == 1) {
      for (i = 0; i < 5; i++) {
        if (order[i] == 3) {
          break;
        }
      }
      if (((MENU->battle.unk_294[side] >> (i * 3)) & 7) == 0) {
        if (i + 1 == 5) {
          tmp = order[i];
          order[i] = order[0];
          order[0] = tmp;
        } else {
          tmp = order[i];
          order[i] = order[i + 1];
          order[i + 1] = tmp;
        }
      }
    }
    MENU->battle.unk_298[side] = PACK(order);

    for (i = 0; i < 5; i++) {
      order[i] = i;
    }
    for (i = 0; i < 5; i++) {
      j = HwMod32(MainLoop_GetFrameCount() >> (i + side + 2), 5);
      tmp = order[i];
      order[i] = order[j];
      order[j] = tmp;
    }
    MENU->battle.unk_29c[side] = PACK(order);

    used = 0;
    for (slot = 0; slot < 5; slot++) {
      order[slot] = 0;
      best = -1;
      for (k = 0, shift = 0; k < 5; shift += 3, k++) {
        if (!(used & (1 << k))) {
          if (best < ((MENU->battle.unk_294[side] >> shift) & 7)) {
            best = (MENU->battle.unk_294[side] >> shift) & 7;
            order[slot] = k;
          }
        }
      }
      used |= 1 << order[slot];
    }
    MENU->battle.unk_2a0[side] = PACK(order);
  }
}
