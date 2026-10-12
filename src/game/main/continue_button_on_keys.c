#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_0204ccc0(MenuButton* button);

/* Key handler of the button that func_0204cc54 adds (data_0215afb4, 32 by
 * 32 at 224, 160): A, B or Start run its confirm handler (func_0204ccc0). */
void ContinueButton_OnKeys(MenuButton* button) {
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    func_0204ccc0(button);
  }
}
