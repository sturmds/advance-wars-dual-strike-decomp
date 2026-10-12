#include "types.h"
#include "game/ov005_task.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba0.h"

void Ov005_EnableCoListTouch(Ov005Task* task) {
  Unk_ov005_0236bba0* work = task->work;
  int i;

  for (i = 0; i < 33; i++) {
    if (work->entryActive[i] != 0) {
      MenuButton* button = MenuButton_Find(work->buttonIds[i]);
      button->flags &= ~MENU_BUTTON_NO_TOUCH_CONFIRM;
    }
  }
}
