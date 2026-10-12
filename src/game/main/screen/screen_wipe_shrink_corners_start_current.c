#include "types.h"
#include "game/task.h"
#include "game/screen_wipe.h"

int Screen_GetNibble(void);
void Screen_SetNibble(int value);
void func_02007710(int value);
void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);

/* Like ScreenWipe_ShrinkCornersStart, for the current screen only. Ends the
 * task when that screen is already shown (nibble 0). */
void ScreenWipe_ShrinkCornersStartCurrent(Task* task) {
  if (Screen_GetNibble() == 0) {
    Task_Destroy(task);
    return;
  }
  Screen_SetNibble(0);
  func_02007710(0);
  task->unk_64 = 1;
  Screen_EnableEmptyWindows();
  Screen_ResetWindowRects();
  data_0216e29c = 352;
  data_0216e2a8 = 0;
}
