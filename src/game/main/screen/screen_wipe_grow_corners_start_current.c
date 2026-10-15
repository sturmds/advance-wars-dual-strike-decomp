#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"
#include "game/screen.h"

void func_02007710(int value);

/* Like ScreenWipe_GrowCornersStart, for the current screen only. Ends the
 * task when that screen is already hidden (nibble 1). */
void ScreenWipe_GrowCornersStartCurrent(Task* task) {
  if (Screen_GetNibble() == 1) {
    Task_Destroy(task);
    return;
  }
  Screen_SetNibble(1);
  func_02007710(0);
  task->unk_64 = 1;
  Screen_EnableEmptyWindows();
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = 0;
  data_027c0280->win0Bottom = 0;
  data_027c0280->win1Left = 0;
  data_027c0280->win1Top = 0;
  data_027c0280->win1Right = 0;
  data_027c0280->win1Bottom = 0;
  data_0216e29c = 0;
  data_0216e2a8 = 0;
}
