#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Moves the cursor to a row button (ids 0x80 up). */
void Ov005_SetShopCursorRow(MenuButton* button) {
  Unk_ov005_0236bbb0* work = data_ov005_0236bbb0;
  work->cursorRow = button->id - 0x80;
  work->savedCursorRow = work->cursorRow;
}
