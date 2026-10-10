#include "types.h"
#include "game/menu_state.h"

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

int func_ov002_0235972c(int side, int slot, int entry);
int func_ov002_02359674(int side);
void BattleSpriteTask_CreateDefaultWrapper(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);

/* Makes the fighter objects of both sides: one per entry with unk_14 set (or one for the
 * side when its animation data has unk_48 set), then the side's start effect when its
 * gMenuState slot at 0x68 is filled. Partial (88.8%): the original reloads the base of
 * gMenuState for the animation data, ours reuses the register of the 0x68 check. */
void func_ov002_023571d0(void)
{
    int side;
    int slot;
    BattleRow *row = (BattleRow *)&gMenuState;

    for (side = 0; side < 2; side++, row++) {
        if (BATTLE_SIDE_ANIM(side)->unk_48 == 0) {
            BattleEntryAt310 *at = (BattleEntryAt310 *)row;

            for (slot = 0; slot < 5; slot++) {
                if (at->entry.fillBefore == 2) {
                    at->entry.unk_00 = func_ov002_0235972c(side, slot, (gMenuState.battle.unk_298[side] >> (slot * 3)) & 7);
                } else if (at->entry.fillBefore != 0) {
                    at->entry.unk_00 = func_ov002_0235972c(side, slot, -1);
                }
                at = (BattleEntryAt310 *)((u8 *)at + sizeof(BattleEntry));
            }
        } else {
            ((BattleEntryAt310 *)row)->entry.unk_00 = func_ov002_02359674(side);
        }
        if (((int *)&gMenuState + side)[0x68 / 4] != 0) {
            BattleSideAnim *anim = BATTLE_SIDE_ANIM(side);
            BattleAnimStart *start = anim->unk_44;

            BattleSpriteTask_CreateDefaultWrapper(0, 0, start->unk_18, start->unk_1a, start->unk_1c, 0, 0, 0,
                                                   gMenuState.battle.unk_20[side], side == 0 ? 0 : 2, 0, side);
        }
    }
}
