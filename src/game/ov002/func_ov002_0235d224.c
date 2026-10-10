#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"

extern MenuState gMenuState;
extern Unk_027c0280 *data_027c0280;
extern const TaskScriptCmd data_ov002_02369664[];

BOOL Battle_IsSideAir(int side);
void Battle_PutSideHpBar(u16 *tilemap, u8 side, u8 isAir);
void Battle_PutSideHpBlock(u16 *tilemap, u8 side, u8 isAir);
void func_ov002_0235e608(u16 *tilemap, u8 side, u8 isAir);
void func_ov002_0235cd64(int a0);
void func_ov002_0235d4a8(const u16 *src, u16 *dst, int width, int height);
void Gfx_RequestLayerCopy(int a);

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define BATTLE_FIELD(field) ((u8 *)&gMenuState + (u32)&((MenuState *)0)->battle.field)

/* Per-frame update of the battle screen. While the frame counter is 25 to 40 it
 * copies columns of the tile map buffer (gMenuState.battle.unk_7d0) into layer 1
 * from both edges. Then it steps the HP drain of each side that has one running,
 * redrawing the HP bar and block. When the screen counters match it draws the
 * final blocks and starts the task at data_ov002_02369664.
 * WIP: 99.3%, only the register numbers of side, side * 12 and side + gMenuState
 * differ in the first loop. */
void func_ov002_0235d224(void)
{
    int side;
    int isAir;
    int count;

    if (data_0229fb30.unk_10 <= 40 && data_0229fb30.unk_10 >= 25) {
        count = data_0229fb30.unk_10 - 25;
        func_ov002_0235d4a8((u16 *)BATTLE_FIELD(unk_7d0) + (15 - count), data_027c0280->layer1Map, count, 24);
        func_ov002_0235d4a8((u16 *)((u8 *)&gMenuState + 0x7f2), data_027c0280->layer1Map + (32 - count), count, 24);
        Gfx_RequestLayerCopy(1);
    }
    for (side = 0; side < 2; side++) {
        if (gMenuState.battle.unk_1000[side] != 0 && gMenuState.battle.unk_1008[side] != gMenuState.battle.hpFrames[side]) {
            gMenuState.battle.unk_1008[side]++;
            if (gMenuState.battle.hpStart[side] != Ov002_gBattleScene->sides[side].hpAfter) {
                gMenuState.battle.hpShown[side] -= gMenuState.battle.hpStep[side];
                gMenuState.battle.hpStart[side] = (u32)gMenuState.battle.hpShown[side] >> 16;
                isAir = Battle_IsSideAir(side) ? 1 : 0;
                Battle_PutSideHpBar(data_027c0280->layer1Map, side, isAir);
                Battle_PutSideHpBlock(data_027c0280->layer1Map, side, isAir);
                Gfx_RequestLayerCopy(1);
                if (gMenuState.battle.unk_1008[side] == gMenuState.battle.hpFrames[side]
                    && gMenuState.battle.hpStart[side] != Ov002_gBattleScene->sides[side].hpAfter) {
                    gMenuState.battle.unk_1008[side]--;
                }
            }
        }
    }
    if (data_0229fb30.unk_02 == 0) {
        return;
    }
    if (data_0229fb30.unk_08 != data_0229fb30.unk_0c) {
        return;
    }
    if (data_0229fb30.unk_0a != data_0229fb30.unk_0e) {
        return;
    }
    for (side = 0; side < 2; side++) {
        isAir = Battle_IsSideAir(side) ? 1 : 0;
        func_ov002_0235e608(data_027c0280->layer1Map, side, isAir);
    }
    data_0229fb30.unk_02 = 0;
    func_ov002_0235cd64(data_0229fb30.unk_03);
    Gfx_RequestLayerCopy(1);
    Task_Create(data_ov002_02369664, TASK_LIST(3));
}
