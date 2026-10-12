#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Task_Destroy(void* task);

void Ov004_DestroySlotTask(Unk_ov004_022bf158* state, int slot) {
  if (state->slotTasks[slot] == 0) {
    return;
  }
  Task_Destroy(state->slotTasks[slot]);
  state->slotTasks[slot] = 0;
}
