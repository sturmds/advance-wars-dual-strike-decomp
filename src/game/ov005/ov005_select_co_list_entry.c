#include "types.h"
#include "game/menu_button.h"
#include "game/keypad.h"
#include "game/Unk_ov005_0236bba0.h"

u8 Ov005_GetCoListEntryRow(int index);

extern Unk_ov005_0236bba0* data_ov005_0236bba0;

void Ov005_SelectCoListEntry(MenuButton* button) {
  int entry = button->id - 0x80;
  Unk_ov005_0236bba0* work = data_ov005_0236bba0;

  work->selectedEntry = entry;
  if ((gKeypad->repeat &
       (PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) == 0) {
    work->cursorRow = Ov005_GetCoListEntryRow(entry);
  }
}
