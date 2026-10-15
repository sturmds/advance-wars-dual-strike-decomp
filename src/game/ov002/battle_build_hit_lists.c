#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

/* Needed to match: the compiler adds the offset of hitLists at run time. */
#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

#define TIMES ((const BattleHitTimes*)gMenuState.battle.drainProfile[0])

/* Builds the hit list of each side that hits back (BattleSide.counters). Each of the
 * side's five entries goes into the list when its flag is set: fillBefore of the entry
 * for the attacking side, fillAfter for the other. The key is TIMES->unk_04 plus the
 * entry's offset from the table for that role. Needed to match: entry is an int. */
void Battle_BuildHitLists(void) {
  int slot;
  int side;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    BattleHitList* list = (BattleHitList*)BATTLE_FIELD(hitLists) + side;

    list->cursor = 0;
    list->count = 0;
    if (Ov002_gBattleScene->sides[side].counters != 0) {
      for (slot = 0; slot < BATTLE_CELL_COUNT; slot++) {
        int entry = (gMenuState.battle.unk_29c[side] >> (slot * 3)) & 7;

        if (side == Ov002_gBattleScene->attackerSide &&
            gMenuState.battle.unk_310[side][entry].fillBefore != 0) {
          list->flags[list->count].cell = entry;
          list->key[list->count] = TIMES->unk_04 + TIMES->unk_0a[list->count];
          list->count++;
        }
        if (side != Ov002_gBattleScene->attackerSide &&
            gMenuState.battle.unk_310[side][entry].fillAfter != 0) {
          list->flags[list->count].cell = entry;
          list->key[list->count] = TIMES->unk_04 + TIMES->unk_0f[list->count];
          list->count++;
        }
      }
    }
  }
}
