#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void func_ov004_022ba964(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < (int)ARRAY_COUNT(state->slotState); i++) {
    if (state->slotState[i] != 0) {
      state->slotState[i] = 1;
    }
  }
}
