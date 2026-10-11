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

/* Makes the fighter objects of both sides: one per entry with unk_14 set (or one for the
 * side when its animation data has unk_48 set), then the side's start sprite from the
 * table in battle.unk_68 when it is set. Partial (91.9%): the original reads unk_68 as
 * base + side * 4 + 0x68 without sharing that sum with the unk_27c read; reading it as a
 * plain field shares it (90.0%), reading it through `colours` as below stops the sharing
 * but adds the 0x68 before the shift. */
void func_ov002_023571d0(void)
{
    int side;
    int slot;
    const u32 *table;
    BattleRow *row = (BattleRow *)&gMenuState;

    for (side = 0; side < 2; side++, row++) {
        if (BATTLE_SIDE_DATA_AT(side)->unk_48 == 0) {
            BattleEntryAt310 *at = (BattleEntryAt310 *)row;

            for (slot = 0; slot < 5; slot++) {
                if (at->entry.fillBefore == 2) {
                    at->entry.unk_00 = BattleCellTask_Create(side, slot, (gMenuState.battle.unk_298[side] >> (slot * 3)) & 7);
                } else if (at->entry.fillBefore != 0) {
                    at->entry.unk_00 = BattleCellTask_Create(side, slot, -1);
                }
                at = (BattleEntryAt310 *)((u8 *)at + sizeof(BattleEntry));
            }
        } else {
            ((BattleEntryAt310 *)row)->entry.unk_00 = BattleCellTask_CreateFighter(side);
        }
        table = ((const u32 **)gMenuState.colours)[side + 0x68 / 4]; /* battle.unk_68[side] */
        if (table != NULL) {
            BattleSideData *data = gMenuState.battle.unk_27c[side];
            BattleEffectParams *start = data->effects[8];

            BattleSpriteTask_CreateDefaultWrapper(table, 0, start->offset.x, start->offset.y, (s16)start->delay, 0, 0, 0,
                                                   gMenuState.battle.unk_20[side], side == 0 ? 0 : 2, 0, side);
        }
    }
}
