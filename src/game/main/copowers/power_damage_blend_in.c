#include "types.h"
#include "game/power_damage.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Task_ResumeScript(Task* task);

/* Script step after PowerDamage_StartBlend: every third frame fades layer 0
 * in one step (blendEva up, blendEvb down). Resumes the script when
 * blendEva reaches 10. */
void PowerDamage_BlendIn(PowerDamageTask* task) {
  if (task->blendFrame++ > 1) {
    data_027c0280->blendEva++;
    data_027c0280->blendEvb--;
    task->blendFrame = 0;
  }
  if (data_027c0280->blendEva != 10) {
    return;
  }
  Task_ResumeScript((Task*)task);
}
