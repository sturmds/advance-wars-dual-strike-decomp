#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Task_Destroy(struct Task* task);

void Ov004_DestroyChildTasks(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < (int)ARRAY_COUNT(state->childTasks); i++) {
    if (state->childTasks[i] != NULL) {
      Task_Destroy(state->childTasks[i]);
      state->childTasks[i] = NULL;
    }
  }
}
