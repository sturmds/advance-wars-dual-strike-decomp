#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen.h"

int func_02007710(int screens);
void Task_Destroy(Task* task);

/* Script step: like BlendFadeToBlack_Start, but for the current screen only.
 * When the screen is shown, marks it hidden, turns on the darken blend at
 * level 0 and sets up ScreenBlendDarken_Update to raise it by 1/16 a frame.
 * Otherwise the screen is already hidden and the task ends. */
void ScreenBlendDarken_Start(Task* task) {
  if (Screen_GetNibble() == 1) {
    Task_Destroy(task);
    return;
  }
  Screen_SetNibble(1);
  func_02007710(0);
  Screen_ResetBlendAllTop();
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEvy = 0;
  task->unk_64 = 256;
  task->unk_66 = 0;
  task->unk_68 = -1;
}
