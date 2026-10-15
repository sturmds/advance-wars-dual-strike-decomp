#include "types.h"
#include "game/menu_state.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

/* Loads the picture of a CO (id in the low 7 bits; 0 clears dest) to dest,
 * 18 tiles, and its palette in the colours of the given army. */
void Co_LoadArmyMediumPicture(int co, void* dest, int paletteSlot, int army);
void VsSetup_DrawSubSlotPanels(void);
void VsSetup_AddSettingButtons(void);
void VsSetup_ShowSettingHelp2(void);

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* CO pictures of the player slots: 18 tiles and one
 * OBJ palette each, front CO then back CO. */
#define CO_PICTURE_TILE(n) (552 + (n) * 18)
#define CO_PICTURE_PALETTE(n) (18 + (n))

/* Shows the settings menu of the Vs setup screen: loads the front and back
 * CO pictures of each player slot, then adds the setting row buttons
 * (ids 60-68), selects the current row and shows its help text. */
void VsSetup_ShowSettingsMenu(void) {
  int player;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    if (MENU->setup.cos[player][0] != 0) {
      Co_LoadArmyMediumPicture(
          MENU->setup.cos[player][0],
          data_027c0280->objVram + CO_PICTURE_TILE(player * 2) * 32,
          CO_PICTURE_PALETTE(player * 2), player + 1);
    }
    if (MENU->setup.cos[player][1] != 0) {
      Co_LoadArmyMediumPicture(
          MENU->setup.cos[player][1],
          data_027c0280->objVram + CO_PICTURE_TILE(player * 2 + 1) * 32,
          CO_PICTURE_PALETTE(player * 2 + 1), player + 1);
    }
  }
  Screen_SetCurrentGfx(saved);
  VsSetup_DrawSubSlotPanels();
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  MenuButton_Select(MENU->setup.settingRow + 60);
  VsSetup_AddSettingButtons();
  MenuButtons_DisableTouch();
  VsSetup_ShowSettingHelp2();
}
