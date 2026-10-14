#include "types.h"
#include "game/ov005_task.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba0.h"

extern const MenuButtonTemplate data_ov005_0236a550;
void func_ov005_0235cee4(void);

static MenuButtonTemplate data_ov005_0236a4b0 = {
#include "data/ov005/data_ov005_0236a4b0.inc.c"
};

/* Shows button 243 while the list is scrolled down (scroll above 0) and
 * button 244 while it can still scroll further (scroll below 64), on the
 * top screen state. */
void Ov005_UpdateScrollButtons(Ov005Task* task, int scroll) {
  Unk_ov005_0236bba0* work = task->work;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (scroll <= 0) {
    if (work->button243Added != 0) {
      MenuButton_Remove(243);
    }
    work->button243Added = 0;
  } else {
    if (work->button243Added == 0) {
      MenuButton_Add(0, 0, &data_ov005_0236a550, 243);
    }
    work->button243Added = 1;
  }
  if (scroll >= 64) {
    if (work->button244Added != 0) {
      MenuButton_Remove(244);
    }
    work->button244Added = 0;
  } else {
    if (work->button244Added == 0) {
      MenuButton_Add(0, 0, &data_ov005_0236a4b0, 244);
    }
    work->button244Added = 1;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
