#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba8.h"

s16 func_ov005_02362bf0(Unk_ov005_0236bba8* work, int entry);

extern Unk_ov005_0236bba8* data_ov005_0236bba8;

void Ov005_OnCoGridButtonConfirm(MenuButton* button) {
  int entry = button->id - 0x80;
  Unk_ov005_0236bba8* work = data_ov005_0236bba8;

  work->cursor = entry;
  work->coId = func_ov005_02362bf0(work, entry);
  work->gridConfirmTouched = 1;
}
