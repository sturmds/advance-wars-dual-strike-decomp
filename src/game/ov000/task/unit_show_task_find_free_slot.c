#include "types.h"
#include "game/Unk_02080440.h"

#define UNIT_SHOW_SLOT_COUNT 2

/* Returns the first slot no unit show task has taken, or -1. */
s16 UnitShowTask_FindFreeSlot(void) {
  u16 slot;

  for (slot = 0; slot < UNIT_SHOW_SLOT_COUNT; slot++) {
    if (data_027c027c->unk_58[slot] == 0) {
      return slot;
    }
  }
  return -1;
}
