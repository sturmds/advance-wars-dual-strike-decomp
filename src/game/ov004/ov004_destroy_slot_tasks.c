#include "types.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

void Task_Destroy(Task* task);

void Ov004_DestroySlotTasks(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < (int)ARRAY_COUNT(state->slotState); i++) {
    if (state->slotState[i] != 0) {
      Task_Destroy(state->slotTask[i]);
      state->slotTask[i] = 0;
    }
  }
}
