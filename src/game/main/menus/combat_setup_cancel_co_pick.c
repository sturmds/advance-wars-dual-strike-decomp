#include "types.h"
#include "game/menu_button.h"

void MenuButton_DisableTouch(int id);
void Sound_PlaySe(u32 soundId);
void func_020b9864(void);

/* Leaves the CO pick phase of the Combat setup screen (link play): plays the
 * cancel sound, closes the Ready and Unready buttons (ids 254 and 252),
 * turns off touch on the row buttons 20 and 21, and clears the flag that
 * func_020b9864 clears for the screen's caller. */
void CombatSetup_CancelCoPick(void) {
  Sound_PlaySe(0x10009);
  MenuButton_Close(254);
  MenuButton_Close(252);
  MenuButton_DisableTouch(20);
  MenuButton_DisableTouch(21);
  func_020b9864();
}
