#include "types.h"
#include "game/task.h"
#include "game/screen.h"

int func_02007710(int screens);
void Task_Destroy(Task* task);

/* Script step: when the current screen is shown, marks it hidden and sets up
 * ScreenFadeToWhiteSpeed_Update to lighten it by 1/16 of the range a frame.
 * Otherwise the screen is already hidden and the task ends. */
void ScreenFadeToWhiteSpeed_Start(Task* task) {
  if (Screen_GetNibble() == 1) {
    Task_Destroy(task);
    return;
  }
  Screen_SetNibble(1);
  func_02007710(0);
  task->unk_64 = 256;
  task->unk_66 = 4096;
  task->unk_54 = 0;
}
