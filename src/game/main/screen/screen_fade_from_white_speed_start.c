#include "types.h"
#include "game/task.h"
#include "game/screen.h"

int func_02007710(int screens);
void Task_Destroy(Task* task);

/* Script step: when the current screen is hidden, marks it shown and sets up
 * ScreenFadeFromWhiteSpeed_Update to lighten it by 1/16 of the range a frame.
 * Otherwise there is nothing to fade in and the task ends. */
void ScreenFadeFromWhiteSpeed_Start(Task* task) {
  if (Screen_GetNibble() == 0) {
    Task_Destroy(task);
    return;
  }
  Screen_SetNibble(0);
  func_02007710(0);
  task->unk_64 = 256;
  task->unk_66 = 4096;
  task->unk_54 = 0;
}
