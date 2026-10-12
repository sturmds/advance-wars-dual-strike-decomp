#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Task_Destroy(Task* task);

/* Task update: like BlendFadeToBlack_Update, but for the current screen
 * only. Raises the darken level (unk_66, in 1/256 steps) by the speed unk_64,
 * up to 16 << 8 (black). The task ends once the level is 16. */
void ScreenBlendDarken_Update(Task* task) {
  if (data_027c0280->blendEvy == 16) {
    Task_Destroy(task);
    return;
  }
  task->unk_66 += task->unk_64;
  if (task->unk_66 >= 16 << 8) {
    task->unk_66 = 16 << 8;
  }
  data_027c0280->blendEvy = task->unk_66 >> 8;
}
