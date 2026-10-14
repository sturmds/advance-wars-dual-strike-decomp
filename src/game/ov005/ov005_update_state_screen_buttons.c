#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba4.h"
#include "game/task.h"

void Ov005StateScreen_OnBTouched(void);
void Ov005StateScreen_OnPrevTouched(void);
void Ov005StateScreen_OnRTouched(void);
void Ov005_AllocStateWork(void);
void Ov005_FreeStateWork(void);
void Ov005_SetUpStateScreen(void);
void Ov005_UpdateLeftTouched(void);
void Ov005_UpdateRightTouched(void);
void Ov005_UpdateStateScreen(void);

static MenuButtonTemplate data_ov005_0236a6fc = {
#include "data/ov005/data_ov005_0236a6fc.inc.c"
};
MenuButtonTemplate data_ov005_0236a6d4 = {
#include "data/ov005/data_ov005_0236a6d4.inc.c"
};
MenuButtonTemplate data_ov005_0236a6ac = {
#include "data/ov005/data_ov005_0236a6ac.inc.c"
};
MenuButtonTemplate data_ov005_0236a684 = {
#include "data/ov005/data_ov005_0236a684.inc.c"
};
static MenuButtonTemplate data_ov005_0236a65c = {
#include "data/ov005/data_ov005_0236a65c.inc.c"
};
TaskScriptCmd data_ov005_0236a724[] = {
#include "data/ov005/data_ov005_0236a724.inc.c"
};

/* Shows the Left button (245) while the scroll is above 0 and the Right
 * button (246) while it is below the current state's scrollMax, on the top
 * screen state. Removing a button also clears its touched flag. */
void Ov005_UpdateStateScreenButtons(Unk_ov005_0236bba4* work, int scroll) {
  Unk_ov005_0236bba4_State* state = &work->stateTable[work->state];
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (scroll <= 0) {
    if (work->button245Shown != 0) {
      MenuButton_Remove(245);
      work->leftTouched = 0;
    }
    work->button245Shown = 0;
  } else {
    if (work->button245Shown == 0) {
      MenuButton_Add(0, 0, &data_ov005_0236a65c, 245);
    }
    work->button245Shown = 1;
  }
  if (scroll >= state->scrollMax >> 12) {
    if (work->button246Shown != 0) {
      MenuButton_Remove(246);
      work->rightTouched = 0;
    }
    work->button246Shown = 0;
  } else {
    if (work->button246Shown == 0) {
      MenuButton_Add(0, 0, &data_ov005_0236a6fc, 246);
    }
    work->button246Shown = 1;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
