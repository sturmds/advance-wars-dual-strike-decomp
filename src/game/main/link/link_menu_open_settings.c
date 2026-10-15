#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/screen_other_map.h"

void func_02082a8c(int palette);
void func_02088528(int a0);
void MenuButton_DisableTouch(int id);
void LinkMenu_DrawHelpText(void);

extern u8 data_02160af0
    []; /* x, y of each of the 5 entries; needed to match: not const, read again for each button */
extern const MenuButtonTemplate data_02160c00;
extern const MenuButtonTemplate data_02160d18;
extern const MenuButtonTemplate data_02160d68;

/* Opens the link menu settings: sets palette 19 on the sub screen, moves
 * the cursor off screen and selects entry 0. Player 0 gets three buttons
 * per entry i (ids i + 4, i + 8, i + 12), with touch off; the others start
 * func_02088528. Then shows the selected entry's help text. The name is a
 * guess. */
void LinkMenu_OpenSettings(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;
  int i;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02082a8c(19);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Select(0);
  if (data_0229fb30.unk_45a != 0) {
    func_02088528(0);
  }
  if (data_0229fb30.unk_45a == 0) {
    for (i = 0; i < LINK_SETTING_COUNT; i++) {
      MenuButton_Add(data_02160af0[i * 2], data_02160af0[i * 2 + 1],
                     &data_02160d68, i + 4);
      MenuButton_Add(data_02160af0[i * 2], data_02160af0[i * 2 + 1],
                     &data_02160c00, i + 8);
      MenuButton_Add(data_02160af0[i * 2], data_02160af0[i * 2 + 1],
                     &data_02160d18, i + 12);
      MenuButton_DisableTouch(i + 4);
      MenuButton_DisableTouch(i + 8);
      MenuButton_DisableTouch(i + 12);
    }
  }
  LinkMenu_DrawHelpText();
}
