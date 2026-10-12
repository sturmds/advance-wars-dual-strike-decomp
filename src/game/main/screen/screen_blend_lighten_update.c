#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Task_Destroy(Task* task);

/* Task update: like BlendFadeFromBlack_Update, but for the current screen
 * only. Lowers the darken level (unk_66, in 1/256 steps) by the speed
 * unk_64 and ends the task once the level has reached 0. */
void ScreenBlendLighten_Update(Task* task) {
  if (data_027c0280->blendEvy == 0) {
    Task_Destroy(task);
    return;
  }
  task->unk_66 -= task->unk_64;
  if (task->unk_66 <= 0) {
    task->unk_66 = 0;
  }
  data_027c0280->blendEvy = task->unk_66 >> 8;
}
