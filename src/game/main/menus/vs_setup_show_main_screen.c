#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

/* Loads the picture of a CO (id in the low 7 bits; 0 clears dest) to dest,
 * 18 tiles, and its palette in the colours of the given army. */
void Co_LoadArmyMediumPicture(int co, void *dest, int paletteSlot, int army);
void Screen_SetCurrentBoth(void *state);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void VsSetup_DrawSlotPanels(void);
void VsSetup_AddArrowButtons(void);
void VsSetup_RedrawSkills(void);
void VsSetup_RedrawAllSkills(void);
BOOL BattleSetup_IsCursorOnCo(void);
void HelpText_Show(u32 textId, int tile);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* CO pictures of the player slots: 18 tiles and one
 * OBJ palette each, front CO then back CO. */
#define CO_PICTURE_TILE(n) (552 + (n) * 18)
#define CO_PICTURE_PALETTE(n) (18 + (n))

/* Script step that shows the main page of the Vs setup screen (map and CO
 * buttons of each slot) again: runs Gfx_ClearLayerMap and Gfx_RequestLayerCopy
 * on layers 0-2 of both screens, loads the front and back CO pictures of
 * each player slot, draws the slot panels, selects the button under the
 * setup cursor, adds the arrow buttons, redraws the skills and shows the
 * help text for a CO or a map button. */
void VsSetup_ShowMainScreen(void)
{
    int player;
    Unk_027c0280 *saved0280;
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedScreen = data_027c027c;

    Screen_SetCurrentBoth(data_027c0288.unk_00);
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Gfx_ClearLayerMap(2);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Gfx_ClearLayerMap(2);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
        if (MENU->setup.cos[player][0] != 0) {
            Co_LoadArmyMediumPicture(MENU->setup.cos[player][0], data_027c0280->objVram + CO_PICTURE_TILE(player * 2) * 32,
                          CO_PICTURE_PALETTE(player * 2), player + 1);
        }
        if (MENU->setup.cos[player][1] != 0) {
            Co_LoadArmyMediumPicture(MENU->setup.cos[player][1], data_027c0280->objVram + CO_PICTURE_TILE(player * 2 + 1) * 32,
                          CO_PICTURE_PALETTE(player * 2 + 1), player + 1);
        }
    }
    Screen_SetCurrentGfx(saved0280);

    VsSetup_DrawSlotPanels();
    Menu_Reset();
    MenuCursor_SetRect(0, -16, 0x100, -16);
    MenuButton_Select(MENU->setup.cursorButton);
    VsSetup_AddArrowButtons();
    MenuButtons_DisableTouch();
    VsSetup_RedrawSkills();
    VsSetup_RedrawAllSkills();
    if (BattleSetup_IsCursorOnCo() == TRUE) {
        HelpText_Show(0x7f000018, 0x3c0);
    } else {
        HelpText_Show(0x7f000019, 0x3c0);
    }
}
