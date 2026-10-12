#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);

/* One frame of the fade to black: raises the brightness blend (BLDY) by 1 and
 * resumes the task script once it reaches 16. */
void Battle_FadeOutStep(Task* task) {
  data_027c0280->blendEvy++;
  if (data_027c0280->blendEvy != 16) {
    return;
  }
  Task_ResumeScript(task);
}
