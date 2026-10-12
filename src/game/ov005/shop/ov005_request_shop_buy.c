#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Touch button of a row (ids 0x80 up): moves the cursor to it and asks to
 * buy its item like A. */
int Ov005_RequestShopBuy(MenuButton* button) {
  u8 id = button->id;
  Unk_ov005_0236bbb0* work = data_ov005_0236bbb0;
  int row = id - 0x80;
  work->cursorRow = row;
  work->buyRequested = 1;
  return row;
}
