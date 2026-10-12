#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
int func_0207a260(void);
void func_0207e5e4(void);

/* Leaves the two-player link menu (stage 0). Button 254 is closed on the side where
 * func_0207a260() is 0, then func_0207e5e4 starts the task that follows. */
void LinkMenu_Exit(void) {
  Sound_PlaySe(0x10009);
  data_0229fb30.unk_630 = 0;
  if (func_0207a260() == 0) {
    MenuButtons_RemoveSelectable();
    MenuButton_Close(254);
  }
  func_0207e5e4();
}
