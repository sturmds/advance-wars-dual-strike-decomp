#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_ResetControl(Unk_ov004_022bf158* state) {
  int i;

  state->controlTask = 0;
  state->controlRunning = 0;
  for (i = 0; i < OV004_EASE_COUNT; i++) {
    OV004_EASE_VALUES(state)[i] = 0;
  }
}
