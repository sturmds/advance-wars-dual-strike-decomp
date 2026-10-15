#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"

void Battle_SortHitList(BattleHitList* list);

#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

#define TIMES(i) ((const BattleHitTimes*)gMenuState.battle.drainProfile[i])
/* TIMES(i), storing the address of the profile pointer in p on the way. */
#define TIMES_VIA(p, i) \
  ((const BattleHitTimes*)*((p) = &gMenuState.battle.drainProfile[i]))

/* Builds the step list of each side (stepLists) from the
 * other side's hit list: first one step per hit, taking entries from unk_2a0 in turn (those
 * with fillBefore set, or all when unk_48 is 1), then one step for each entry whose fill
 * changes and is not in the list yet. Keys come from the drain profiles; the list is then
 * sorted by key. */
void func_ov002_023552d0(void) {
  int entry;
  int side;
  int n;
  int slot;
  u8 limit;
  int other;
  int j;
  int i;
  int stepEntry;
  const void** profile;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    BattleHitList* list = (BattleHitList*)BATTLE_FIELD(stepLists) + side;

    list->cursor = 0;
    list->count = 0;
    other = (side + 1) & 1;
    limit = gMenuState.battle.hitLists[other].count;
    if (limit == 0) {
      continue;
    }

    slot = 0;
    while (list->count != limit) {
      entry = (gMenuState.battle.unk_2a0[side] >> (slot * 3)) & 7;
      if (gMenuState.battle.unk_310[side][entry].fillBefore != 0 ||
          gMenuState.battle.unk_27c[side]->unk_48 == 1) {
        list->flags[list->count].cell = entry;
        list->flags[list->count].unk_3 =
            (gMenuState.battle.unk_29c[other] >> (list->count * 3)) & 7;
        list->flags[list->count].unk_6 = 1;
        list->flags[list->count].unk_7 = 0;
        list->count++;
      }
      slot++;
      if (slot == 5) {
        slot = 0;
      }
    }

    for (i = 0; i < list->count; i++) {
      if (side == Ov002_gBattleScene->attackerSide) {
        list->key[i] = TIMES(0)->unk_04 + TIMES(0)->unk_0f[i] +
                       TIMES(1)->unk_06 - TIMES(1)->unk_04;
      } else {
        /* Needed to match: the byte goes through j before the add. */
        j = TIMES(0)->unk_0a[i];
        list->key[i] = j + TIMES(0)->unk_06;
      }
    }

    for (i = 0; i < list->count; i++) {
      stepEntry = list->flags[i].cell;
      if (gMenuState.battle.unk_310[side][stepEntry].fillAfter != 0 ||
          gMenuState.battle.unk_27c[side]->unk_48 == 1) {
        list->flags[i].unk_6 = 0;
      } else {
        for (j = i + 1; j < list->count; j++) {
          if (stepEntry == list->flags[j].cell) {
            list->flags[i].unk_6 = 0;
            break;
          }
        }
      }
    }

    n = 0;
    for (slot = 0; slot < BATTLE_CELL_COUNT; slot++) {
      entry = (gMenuState.battle.unk_2a0[side] >> (slot * 3)) & 7;
      if (gMenuState.battle.unk_310[side][entry].fillBefore ==
          gMenuState.battle.unk_310[side][entry].fillAfter) {
        continue;
      }
      for (j = 0; j < list->count; j++) {
        if (entry == list->flags[j].cell) {
          goto next;
        }
      }
      list->flags[list->count].cell = entry;
      list->flags[list->count].unk_7 = 1;
      if (gMenuState.battle.unk_310[side][entry].fillAfter == 0) {
        list->flags[list->count].unk_6 = 1;
      } else {
        list->flags[list->count].unk_6 = 0;
      }
      if (side == Ov002_gBattleScene->attackerSide) {
        /* Needed to match: the last two terms as `- -x`, and TIMES_VIA on the last one;
         * a plain sum adds the terms in another order and loads unk_08 earlier. */
        list->key[list->count] = TIMES(0)->unk_0f[n] - TIMES(1)->unk_04 -
                                 (-TIMES(0)->unk_04) -
                                 (-TIMES_VIA(profile, 1)->unk_08);
      } else {
        j = TIMES(0)->unk_0a[n];
        list->key[list->count] = j + TIMES(0)->unk_08;
      }
      list->flags[list->count].unk_3 = 7;
      n++;
      list->count++;
    next:;
    }
    Battle_SortHitList(list);
  }
}
