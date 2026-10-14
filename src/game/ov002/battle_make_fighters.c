#include "types.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

/* Needed to match: the entries of a side are reached from gMenuState by side index
 * (0x78 bytes each) and entry index (0x18 bytes each) first, then by their offset 0x310. */
typedef struct BattleRow {
  u8 bytes[0x78];
} BattleRow;

typedef struct BattleEntryAt310 {
  u8 pad_000[0x310];
  BattleEntry entry;
} BattleEntryAt310;

int BattleCellTask_Create(int side, int slot, int entry);
int BattleCellTask_CreateFighter(int side);

/* The effect parameters of a side's start sprite. Needed to match: read in place at each
 * use, so the compiler does not share side * 4 with the battle.unk_68 read. */
#define START_EFFECT(side) (gMenuState.battle.unk_27c[side]->effects[8])

/* Makes the fighter objects of both sides: one per entry with unk_14 set (or one for the
 * side when its animation data has unk_48 set), then the side's start sprite from the
 * table in battle.unk_68 when it is set. */
void Battle_MakeFighters(void) {
  int side;
  int slot;
  const u32* table;
  BattleRow* row = (BattleRow*)&gMenuState;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++, row++) {
    if (BATTLE_SIDE_DATA_AT(side)->unk_48 == 0) {
      BattleEntryAt310* at = (BattleEntryAt310*)row;

      for (slot = 0; slot < BATTLE_CELL_COUNT; slot++) {
        if (at->entry.fillBefore == 2) {
          at->entry.unk_00 = BattleCellTask_Create(
              side, slot, (gMenuState.battle.unk_298[side] >> (slot * 3)) & 7);
        } else if (at->entry.fillBefore != 0) {
          at->entry.unk_00 = BattleCellTask_Create(side, slot, -1);
        }
        at = (BattleEntryAt310*)((u8*)at + sizeof(BattleEntry));
      }
    } else {
      ((BattleEntryAt310*)row)->entry.unk_00 =
          BattleCellTask_CreateFighter(side);
    }
    table = gMenuState.battle.unk_68[side];
    if (table != NULL) {
      BattleSpriteTask_CreateDefaultWrapper(
          table, 0, START_EFFECT(side)->offset.x, START_EFFECT(side)->offset.y,
          (s16)START_EFFECT(side)->delay, 0, 0, 0,
          gMenuState.battle.unk_20[side], side == 0 ? 0 : 2, 0, side);
    }
  }
}
