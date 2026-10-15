#include "types.h"
#include "sdk/gx.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/window_wipe.h"
#include "game/screen.h"
#include "game/screen_wipe.h"

void func_02007710(int value);
void DmaTask_Start(const void* src, volatile void* dest, int unk34, int unk36,
                   void (*callback)(void));

/* Like WindowWipe_StartShow, for the current screen only. Ends the task when
 * that screen is already shown (nibble 0). */
void WindowWipe_StartShowCurrent(Task* task) {
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
  DmaTask_Start(
      gWipeWin0hTable,
      data_027c027c == data_027c0288.unk_00 ? REG_WIN0H : REG_DB_WIN0H, 1,
      0xa240, WindowWipe_BuildEdgesIn);
}
