#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Task_Destroy(struct Task* task);

void Ov004_DestroyControlTask(Unk_ov004_022bf158* state) {
  if (state->controlTask == 0) {
    return;
  }
  Task_Destroy(state->controlTask);
  state->controlTask = 0;
}
