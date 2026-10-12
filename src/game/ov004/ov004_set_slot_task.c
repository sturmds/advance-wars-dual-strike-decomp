#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_SetSlotTask(Unk_ov004_022bf158* state, Ov004Task* task, int slot) {
  state->slotTasks[slot] = task;
  state->unk_998[slot] = 1;
  task->slot = slot;
}
