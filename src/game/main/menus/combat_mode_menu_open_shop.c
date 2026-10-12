#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_02160ac0.h"

void Font_LoadDigits(int baseTile);
void Co_UnpackPicture(int coId, int firstTile);
void HelpText_Show(u32 textId, int tile);
void CombatModeMenu_DrawFundsAndPrices(void);
void func_02089c9c(void);
void Menu_DrawSelectedHelpText(void);
void MenuButton_DisableTouch(int id);

extern u8 gCombatModeCos[];
extern const MenuButtonTemplate data_02161470;

/* First step of the Combat mode shop script (data_02161570), the
 * counterpart of CombatModeMenu_ShowMainLayers: clears the buttons, turns
 * BG0, BG1, BG3 and OBJ on and BG2 off on the main screen, loads the digit
 * font and the picked CO's picture, selects the first shop entry (button 8)
 * and draws the funds, prices and help texts. Adds button 13, which
 * cannot be touched. */
void CombatModeMenu_OpenShop(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  MenuButtons_RemoveAll();
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  Font_LoadDigits(0);
  Co_UnpackPicture(gCombatModeCos[data_02160ac0->co], 0x54);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  MenuButton_Select(8);
  HelpText_Show(0x44000001, 0x3c0);
  CombatModeMenu_DrawFundsAndPrices();
  func_02089c9c();
  Menu_DrawSelectedHelpText();
  MenuButton_Add(0, 0, &data_02161470, 13);
  MenuButton_DisableTouch(13);
}
