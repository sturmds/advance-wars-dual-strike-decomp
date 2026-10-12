#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void ResultScreen_Confirm(MenuButton* button);

/* Script step: confirms the result screen when A is pressed. */
void ResultSprite_WaitForA(void) {
  if (gKeypad->pressed & PAD_BUTTON_A) {
    ResultScreen_Confirm(NULL);
  }
}
