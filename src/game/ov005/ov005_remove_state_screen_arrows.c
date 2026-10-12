#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba4.h"

void Ov005_RemoveStateScreenButtons(Unk_ov005_0236bba4* work) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (work->button245Shown != 0) {
    MenuButton_Remove(245);
    work->button245Shown = 0;
  }
  if (work->button246Shown != 0) {
    MenuButton_Remove(246);
    work->button246Shown = 0;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
