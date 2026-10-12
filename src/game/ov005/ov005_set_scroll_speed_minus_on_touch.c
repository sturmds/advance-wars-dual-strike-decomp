#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba0.h"

extern Unk_ov005_0236bba0* data_ov005_0236bba0;

void Ov005_SetScrollSpeedMinusOnTouch(MenuButton* button) {
  u8 touchState = button->touchState;
  Unk_ov005_0236bba0* work = data_ov005_0236bba0;

  if ((u32)((touchState + 255) & 255) <= (u32)1) { /* pressed or held */
    work->unk_82c = -0x1000;
  }
}
