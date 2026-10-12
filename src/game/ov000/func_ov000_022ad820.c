#include "types.h"
#include "game/unit_show_task.h"

int func_020cfd30(void);

int func_ov000_022ad820(UnitShowTask* task) {
  int t0 = func_020cfd30();
  if (t0 == 0) {
    task->state = UNIT_SHOW_STATE_DELAY;
    return UNIT_SHOW_STATE_DELAY;
  }
  return t0;
}
