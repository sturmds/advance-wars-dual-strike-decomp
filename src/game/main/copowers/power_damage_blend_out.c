#include "types.h"
#include "game/power_damage.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Task_ResumeScript(Task* task);

/* Script step after PowerDamage_StartBlend: every third frame fades layer 0
 * out one step (blendEva down, blendEvb up). Resumes the script when
 * blendEva reaches 0. */
void PowerDamage_BlendOut(PowerDamageTask* task) {
  if (task->blendFrame++ > 1) {
    data_027c0280->blendEva--;
    data_027c0280->blendEvb++;
    task->blendFrame = 0;
  }
  if (data_027c0280->blendEva != 0) {
    return;
  }
  Task_ResumeScript((Task*)task);
}
