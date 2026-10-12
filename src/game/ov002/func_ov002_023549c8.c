#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"

extern MenuState gMenuState;

void Battle_SortHitList(BattleHitList* list);

#define BATTLE (gMenuState.battle)
#define TIMES(i) ((const BattleHitTimes*)BATTLE.drainProfile[i])
/* TIMES(i), storing the address of the profile pointer in p on the way. */
#define TIMES_VIA(p, i) \
  ((const BattleHitTimes*)*((p) = &BATTLE.drainProfile[i]))
/* Key of the k-th added cell: the attacker's offsets moved onto the other profile's times.
 * Needed to match: the last two terms as `- -x`, and TIMES_VIA with its own local at each
 * use; a plain sum adds the terms in another order. */
#define ATTACKER_KEY(k, p)                                        \
  (TIMES(0)->unk_0f[k] - TIMES(1)->unk_04 - (-TIMES(0)->unk_04) - \
   (-TIMES_VIA(p, 1)->unk_08))
/* Needed to match: an int-sized enum. A plain 0/1 ternary emits its conditional moves in
 * the other order, and a byte-sized enum adds a mask. */
#pragma enumsalwaysint on
enum { ROLE_ATTACKER, ROLE_DEFENDER };
#pragma enumsalwaysint reset
#define ROLE(side) \
  ((side) == Ov002_gBattleScene->attackerSide ? ROLE_ATTACKER : ROLE_DEFENDER)
/* Added to a step list key to move it onto this list's times. */
#define SHIFT \
  (TIMES((ROLE(side) + 1) & 1)->unk_08 - TIMES((ROLE(side) + 1) & 1)->unk_06)

/* Needed to match: the flags of the step list and the new list are reached with the field
 * offset added at run time, the step list's count and keys with it folded into the load. */
#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)
#define STEPS ((BattleHitList*)BATTLE_FIELD(stepLists) + side)
#define LIST ((BattleHitList*)BATTLE_FIELD(unk_2ec) + side)

typedef struct StepListsView {
  u8 pad_000[0x2c8];
  BattleHitList stepLists[2];
} StepListsView;

#define STEP_LIST (((StepListsView*)&gMenuState)->stepLists[side])

/* Builds BattleScreenWork.unk_2ec for each side, sorted by key. When the other side's
 * BattleSideData.effects[4 + weapon] entry is set, the list takes the side's step list
 * entries (unk_6 set only on the last step of a cell whose fill ends at 0), then the cells
 * of unk_2a0 that empty and are not in it yet. Otherwise it takes the emptying cells of
 * unk_2a0, or one entry for cell 0 when BattleSideData.unk_48 is 1 and every cell empties;
 * each entry's key is raised to the latest matching step. Partial match (99.9%): cell
 * swaps registers with the search counter in the unk_2a0 loop of the first branch.
 * The defender key reads its offset into a u8 local first, like func_ov002_023552d0;
 * sum also counts the search loop, as the original's registers show. */
void func_ov002_023549c8(void) {
  int side;
  int i;
  int j;
  int cell;
  int added;
  int sum;
  u8 offset;
  BattleHitFlags* flags;
  const void** profile1;
  const void** profile2;
  const void** profile3;

  for (side = 0; side < 2; side++) {
    LIST->cursor = 0;
    LIST->count = 0;
    /* effects[4 + weapon]; needed to match: indexed from &effects[4]. */
    if ((&BATTLE.unk_27c[(side + 1) & 1]->effects[4])
            [Ov002_gBattleScene->sides[(side + 1) & 1].usesSecondary] != NULL) {
      for (i = 0; i < STEP_LIST.count; i++) {
        if (STEPS->flags[i].unk_7 == 0) {
          LIST->flags[LIST->count].cell = STEPS->flags[i].cell;
          LIST->flags[LIST->count].unk_3 = STEPS->flags[i].unk_3;
          LIST->flags[LIST->count].unk_6 = 1;
          LIST->key[LIST->count] = STEP_LIST.key[i] + SHIFT;
          flags = &LIST->flags[LIST->count];
          cell = flags->cell;
          if (BATTLE.unk_310[side][cell].fillAfter != 0 ||
              BATTLE.unk_27c[side]->unk_48 == 1) {
            flags->unk_6 = 0;
          } else {
            for (j = i + 1; j < STEP_LIST.count; j++) {
              if (cell == STEPS->flags[j].cell) {
                flags->unk_6 = 0;
                break;
              }
            }
          }
          LIST->count++;
        }
      }
      if (BATTLE.unk_27c[side]->unk_48 != 1) {
        added = 0;
        for (i = 0; i < 5; i++) {
          cell = (BATTLE.unk_2a0[side] >> (i * 3)) & 7;

          if (BATTLE.unk_310[side][cell].fillBefore == 0 ||
              BATTLE.unk_310[side][cell].fillAfter != 0) {
            continue;
          }
          for (sum = 0; sum < LIST->count; sum++) {
            if (cell == LIST->flags[sum].cell && LIST->flags[sum].unk_6 == 1) {
              goto next;
            }
          }
          LIST->flags[LIST->count].cell = cell;
          LIST->flags[LIST->count].unk_6 = 1;
          if (side == Ov002_gBattleScene->attackerSide) {
            LIST->key[LIST->count] = ATTACKER_KEY(added, profile1);
          } else {
            offset = TIMES(0)->unk_0a[added];
            LIST->key[LIST->count] = offset + TIMES(0)->unk_08;
          }
          LIST->flags[LIST->count].unk_3 = 7;
          added++;
          LIST->count++;
        next:;
        }
      }
    } else if (BATTLE.unk_27c[side]->unk_48 == 1) {
      sum = 0;
      for (cell = 0; cell < 5; cell++) {
        sum += BATTLE.unk_310[side][cell].fillAfter;
      }
      if (sum == 0) {
        LIST->flags[LIST->count].cell = 0;
        LIST->flags[LIST->count].unk_6 = 1;
        LIST->flags[LIST->count].unk_3 = 7;
        if (side == Ov002_gBattleScene->attackerSide) {
          LIST->key[LIST->count] = ATTACKER_KEY(LIST->count, profile2);
        } else {
          offset = TIMES(0)->unk_0a[LIST->count];
          LIST->key[LIST->count] = offset + TIMES(0)->unk_08;
        }
        for (i = 0; i < STEP_LIST.count; i++) {
          if (STEPS->flags[i].unk_7 == 0 &&
              STEP_LIST.key[i] + SHIFT >= LIST->key[LIST->count]) {
            LIST->key[LIST->count] = STEP_LIST.key[i] + SHIFT;
            LIST->flags[LIST->count].unk_3 = STEPS->flags[i].unk_3;
          }
        }
        LIST->count++;
      }
    } else {
      for (i = 0; i < 5; i++) {
        cell = (BATTLE.unk_2a0[side] >> (i * 3)) & 7;

        if (BATTLE.unk_310[side][cell].fillBefore == 0 ||
            BATTLE.unk_310[side][cell].fillAfter != 0) {
          continue;
        }
        LIST->flags[LIST->count].cell = cell;
        LIST->flags[LIST->count].unk_6 = 1;
        if (side == Ov002_gBattleScene->attackerSide) {
          LIST->key[LIST->count] = ATTACKER_KEY(LIST->count, profile3);
        } else {
          offset = TIMES(0)->unk_0a[LIST->count];
          LIST->key[LIST->count] = offset + TIMES(0)->unk_08;
        }
        LIST->flags[LIST->count].unk_3 = 7;
        for (j = 0; j < STEP_LIST.count; j++) {
          if (STEPS->flags[j].unk_7 == 0 && cell == STEPS->flags[j].cell &&
              STEP_LIST.key[j] + SHIFT >= LIST->key[LIST->count]) {
            LIST->key[LIST->count] = STEP_LIST.key[j] + SHIFT;
            LIST->flags[LIST->count].unk_3 = STEPS->flags[j].unk_3;
          }
        }
        LIST->count++;
      }
    }
    Battle_SortHitList(LIST);
    if (BATTLE.unk_27c[side]->unk_48 == 1) {
      sum = 0;
      for (cell = 0; cell < 5; cell++) {
        sum += BATTLE.unk_310[side][cell].fillAfter;
      }
      if (sum == 0 && LIST->count != 0) {
        LIST->flags[LIST->count - 1].unk_6 = 1;
      }
    }
  }
}
