#include "types.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void MenuButton_DisableTouch(int id);
void func_020b9864(void);

/* Called when a row owner backs out of the shop: plays the cancel sound, closes the shop
 * buttons and clears the state byte that func_020b9864 clears. */
void CombatSetup_LeaveShop(void) {
  Sound_PlaySe(0x10009);
  MenuButton_Close(254);
  MenuButton_Close(252);
  MenuButton_DisableTouch(253);
  MenuButtons_RemoveSelectable();
  func_020b9864();
}
