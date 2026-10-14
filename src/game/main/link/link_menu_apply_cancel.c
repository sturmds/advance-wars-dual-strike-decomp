#include "types.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_Close(int id);
void MenuButton_DisableTouch(int id);
void func_020b9864(void);

/* Link message 5: the menu is cancelled. Player 0 also turns off the touch
 * of the buttons i + 4, i + 8 and i + 12 for entries i = 0-4 and closes
 * buttons 254 and 252. */
void LinkMenu_ApplyCancel(void) {
  int i;

  Sound_PlaySe(0x10009);
  if (data_0229fb30.unk_45a == 0) {
    for (i = 0; i < LINK_SETTING_COUNT; i++) {
      MenuButton_DisableTouch(i + 4);
      MenuButton_DisableTouch(i + 8);
      MenuButton_DisableTouch(i + 12);
    }
    MenuButton_Close(254);
    MenuButton_Close(252);
  }
  func_020b9864();
}
