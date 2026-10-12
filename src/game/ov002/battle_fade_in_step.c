#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);

/* One frame of the fade from black: lowers BLDY by 1. At 0 it turns the
 * blend off, sets the default weights (16 and 0) and resumes the task script. */
void Battle_FadeInStep(Task* task) {
  data_027c0280->blendEvy--;
  if (data_027c0280->blendEvy != 0) {
    return;
  }
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Task_ResumeScript(task);
}
