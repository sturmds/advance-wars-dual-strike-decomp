#include "types.h"
#include "game/ov005_task.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba0.h"

/* Removes the buttons with ids 243 and 244 (which look like scroll arrows:
 * their update callbacks set the scroll speed) from the top screen state. */
void Ov005_RemoveScrollButtons(Ov005Task* task) {
  Unk_ov005_0236bba0* work = task->work;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (work->button243Added != 0) {
    MenuButton_Remove(243);
    work->button243Added = 0;
  }
  if (work->button244Added != 0) {
    MenuButton_Remove(244);
    work->button244Added = 0;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
