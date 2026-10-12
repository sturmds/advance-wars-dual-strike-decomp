#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_ClearSlots(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < OV004_SLOT_COUNT; i++) {
    state->slotTasks[i] = 0;
    state->unk_998[i] = 0;
  }
}
