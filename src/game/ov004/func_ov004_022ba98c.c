#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void func_ov004_022ba98c(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < 24; i++) {
    if (state->slotState[i] != 0) {
      state->slotState[i] = 2;
    }
  }
}
