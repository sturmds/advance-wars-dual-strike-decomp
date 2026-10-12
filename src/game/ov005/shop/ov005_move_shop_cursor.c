#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Up and Down move the cursor over the page's buttons and wrap around at
 * both ends; the new button is selected in the menu. */
void Ov005_MoveShopCursorOnKeys(void) {
  Unk_ov005_0236bbb0* work = data_ov005_0236bbb0;
  int next = work->cursorRow;
  u16 keys = gKeypad->repeat;

  if (keys & PAD_KEY_UP) {
    if (next <= 0) {
      next = work->buttonCount - 1;
    } else {
      next--;
    }
  } else if (keys & PAD_KEY_DOWN) {
    if (next >= work->buttonCount - 1) {
      next = 0;
    } else {
      next++;
    }
  }
  if (next != work->cursorRow) {
    MenuButton_Select(work->buttons[next].buttonId);
    work->savedCursorRow = next;
  }
  work->cursorRow = next;
}
