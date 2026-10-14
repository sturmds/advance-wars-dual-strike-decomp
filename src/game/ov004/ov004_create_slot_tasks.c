#include "types.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

extern const TaskScriptCmd data_ov004_022be818[];
extern const Ov004SlotInit data_ov004_022bea58[];

void Ov004_CreateSlotTasks(Unk_ov004_022bf158* state) {
  int i;
  const Ov004SlotInit* init = data_ov004_022bea58;
  Ov004Task* task;

  for (i = 0; i < OV004_SLOT_TASK_COUNT; i++) {
    task = (Ov004Task*)Task_Create(data_ov004_022be818, TASK_LIST(5));
    task->unk_38 = init->unk_38;
    task->tweenFrom = init->tweenFrom;
    task->tweenTo = init->tweenTo;
    task->tweenFrames = init->tweenFrames;
    task->unk_48 = init->unk_48;
    task->unk_4c = init->unk_4c;
    task->unk_50 = init->unk_50;
    init++;
    state->slotTask[i] = (Task*)task;
    state->slotState[i] = 1;
    task->slot = i;
  }
}
