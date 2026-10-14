#include "types.h"
#include "game/unit_show_task.h"

void UnitShowTask_RunScript(UnitShowTask* task);
BOOL UnitShowTask_CheckVisible(UnitShowTask* task);
void UnitShowTask_PlaceSprite(UnitShowTask* task);

/* Runs each frame. In the move and delay states, once the timer is 0 it
 * runs the next script commands first, which can destroy the task. The
 * `+ 254` is `- 2` as the original wrote it. */
void UnitShowTask_Update(UnitShowTask* task) {
  if (task->state != UNIT_SHOW_STATE_NONE) {
    if ((u8)(task->state + 254) <= 1 && task->waitFrames == 0) {
      UnitShowTask_RunScript(task);
      if (task->taskScript == NULL) {
        return;
      }
    }
    data_ov000_022db6b4[task->state](task);
  }
  if (UnitShowTask_CheckVisible(task)) {
    UnitShowTask_PlaceSprite(task);
  }
}
