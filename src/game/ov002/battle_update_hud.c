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
 * final blocks and starts the task at data_ov002_02369664. */
void Battle_UpdateHud(void)
{
    int side;
    int isAir;
    int n; /* needed to match: the column count, then the side in the HP loop */

    if (data_0229fb30.unk_10 <= 40 && data_0229fb30.unk_10 >= 25) {
        n = data_0229fb30.unk_10 - 25;
        func_ov002_0235d4a8((u16 *)BATTLE_FIELD(unk_7d0) + (15 - n), data_027c0280->layer1Map, n, 24);
        func_ov002_0235d4a8((u16 *)(BATTLE_FIELD(unk_7d0) + 0x22) /* column 17 */, data_027c0280->layer1Map + (32 - n), n, 24);
        Gfx_RequestLayerCopy(1);
    }
    for (n = 0; n < 2; n++) {
        if (gMenuState.battle.unk_1000[n] != 0 && gMenuState.battle.unk_1008[n] != gMenuState.battle.hpFrames[n]) {
            gMenuState.battle.unk_1008[n]++;
            if (gMenuState.battle.hpStart[n] != Ov002_gBattleScene->sides[n].hpAfter) {
                gMenuState.battle.hpShown[n] -= gMenuState.battle.hpStep[n];
                gMenuState.battle.hpStart[n] = (u32)gMenuState.battle.hpShown[n] >> 16;
                isAir = Battle_IsSideAir(n) ? 1 : 0;
                Battle_PutSideHpBar(data_027c0280->layer1Map, n, isAir);
                Battle_PutSideHpBlock(data_027c0280->layer1Map, n, isAir);
                Gfx_RequestLayerCopy(1);
                if (gMenuState.battle.unk_1008[n] == gMenuState.battle.hpFrames[n]
                    && gMenuState.battle.hpStart[n] != Ov002_gBattleScene->sides[n].hpAfter) {
                    gMenuState.battle.unk_1008[n]--;
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
