#include "types.h"
#include "game/menu_button.h"

void MenuButton_DisableTouch(int id);
void func_020b9890(void);

/* Called when every row in use is ready: closes the shop buttons and sets the state byte
 * that func_020b9890 sets. */
void CombatSetup_FinishShop(void) {
  MenuButton_Close(254);
  MenuButton_Close(252);
  MenuButton_DisableTouch(253);
  MenuButtons_RemoveSelectable();
  func_020b9890();
}
