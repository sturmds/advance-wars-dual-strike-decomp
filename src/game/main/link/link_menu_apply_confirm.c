#include "types.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_Close(int id);
void MenuButton_DisableTouch(int id);
void func_020b9890(void);

/* Link message 4: the menu is confirmed. Same as LinkMenu_ApplyCancel, with the
 * other sound and func_020b9890. */
void LinkMenu_ApplyConfirm(void) {
  int i;

  Sound_PlaySe(0x10006);
  if (data_0229fb30.unk_45a == 0) {
    for (i = 0; i < LINK_SETTING_COUNT; i++) {
      MenuButton_DisableTouch(i + 4);
      MenuButton_DisableTouch(i + 8);
      MenuButton_DisableTouch(i + 12);
    }
    MenuButton_Close(254);
    MenuButton_Close(252);
  }
  func_020b9890();
}
