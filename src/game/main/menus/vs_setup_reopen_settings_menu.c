#include "types.h"
#include "game/menu_state.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void *state);
void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void VsSetup_DrawSubSlotPanels(void);
void VsSetup_AddSettingButtonsAndArrows(void);
void VsSetup_ShowSettingHelp(void);

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

/* Settings menu without the CO pictures: runs func_01ff8c84 and a layer copy on layers 0 to 2 of
 * both screens, then adds the row and arrow buttons, selects the current row
 * and shows its help text. The name is a guess. */
void VsSetup_ReopenSettingsMenu(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c = data_027c027c;

    Screen_SetCurrentBoth(data_027c0288.unk_00);
    func_01ff8c84(0);
    func_01ff8c84(1);
    func_01ff8c84(2);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    func_01ff8c84(0);
    func_01ff8c84(1);
    func_01ff8c84(2);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    VsSetup_DrawSubSlotPanels();
    Menu_Reset();
    MenuCursor_SetRect(0, -16, 0x100, -16);
    MenuButton_Select(MENU->setup.settingRow + 60);
    VsSetup_AddSettingButtonsAndArrows();
    MenuButtons_DisableTouch();
    VsSetup_ShowSettingHelp();
}
