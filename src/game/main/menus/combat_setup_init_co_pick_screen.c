#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Co_LoadArmyLargePicture(int coMood, void *dest, int paletteSlot, int army);
void HelpText_Show(u32 textId, int tile);
void func_02088528(int a0);

extern MenuState gMenuState;
extern u8 data_02160ae0[];
extern const MenuButtonTemplate data_02160e80;
extern const MenuButtonTemplate data_02160d90;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Sets up the Combat setup CO pick (link play). Main screen: the second
 * saved map (unk_600) on layer 2 and the small CO picture of each row with
 * memberCount set (palette 18 + row). Sub screen: the saved map on layer 2,
 * scrolled by -16. The row owner gets the row's buttons 20 and 21 (as in
 * CombatSetup_UnlockRow) without touch; any other player hands the row to
 * its owner through func_02088528. Then help text 0x44000012. */
void CombatSetup_InitCoPickScreen(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    Unk_027c0280 *subSavedGfx;
    Unk_02080440 *subSavedState;
    int i;
    int player;
    int row;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Mem_Copy32(MENU->combat.unk_600, data_027c0280->layer2Map, 0x600);
    Gfx_RequestLayerCopy(2);
    for (i = 0; i < 4; i++) {
        if (MENU->combat.rows[i].memberCount != 0) {
            Co_LoadArmyLargePicture(data_02160ae0[LINK_ROWS[i].unk_19], data_027c0280->objVram + (0x400 + i * 36) * 32, i + 18, i + 1);
        }
    }
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    subSavedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    subSavedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Mem_Copy32(&gMenuState, data_027c0280->layer2Map, 0x600);
    Gfx_RequestLayerCopy(2);
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = -16;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -16;
    Screen_SetCurrent(subSavedState);
    Screen_SetCurrentGfx(subSavedGfx);

    MenuCursor_SetRect(0, -16, 256, -16);
    player = data_0229fb30.unk_45a;
    if (player == MENU->combat.rows[MENU->combat.players[player].row - 1].player) {
        row = MENU->combat.players[player].row - 1;
        MenuButton_Add(row * 64 + 16, 96 - row * 16, &data_02160e80, 20);
        row = MENU->combat.players[data_0229fb30.unk_45a].row - 1;
        MenuButton_Add(row * 64 + 16, 96 - row * 16, &data_02160d90, 21);
        MenuButton_DisableTouch(20);
        MenuButton_DisableTouch(21);
    } else {
        func_02088528(MENU->combat.rows[MENU->combat.players[player].row - 1].player);
    }
    HelpText_Show(0x44000012, 960);
}
