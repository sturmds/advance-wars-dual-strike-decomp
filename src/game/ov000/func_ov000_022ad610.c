#include "types.h"
#include "game/unit_show_task.h"

int func_020b98bc(int);

int func_ov000_022ad610(UnitShowTask* task) {
  ShowEntry_PlaySound(task);
  task->state = UNIT_SHOW_STATE_IDLE;
  return func_020b98bc(1);
}
