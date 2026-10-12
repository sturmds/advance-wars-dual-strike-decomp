#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_DestroySlotTask(Unk_ov004_022bf158* state, int slot);

void Ov004_DestroyAllSlotTasks(Unk_ov004_022bf158* state) {
  int slot;

  for (slot = 0; slot < OV004_SLOT_COUNT; slot++) {
    Ov004_DestroySlotTask(state, slot);
  }
}
