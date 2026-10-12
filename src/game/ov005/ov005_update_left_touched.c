#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba4.h"

extern Unk_ov005_0236bba4* data_ov005_0236bba4;

void Ov005_UpdateLeftTouched(MenuButton* button) {
  Unk_ov005_0236bba4* work = data_ov005_0236bba4;

  if (button->touchState == MENU_BUTTON_TOUCH_PRESSED ||
      button->touchState == MENU_BUTTON_TOUCH_HELD) {
    work->leftTouched = 1;
  } else {
    work->leftTouched = 0;
  }
}
