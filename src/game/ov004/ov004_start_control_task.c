#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/task.h"

void Ov004_StartControlTask(Unk_ov004_022bf158* state,
                            const TaskScriptCmd* script) {
  int i;

  state->controlRunning = 1;
  for (i = 0; i < OV004_EASE_COUNT; i++) {
    OV004_EASE_VALUES(state)[i] = 0;
  }
  state->controlTask = Task_Create(script, TASK_LIST(4));
}
