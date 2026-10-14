#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Returns the index of the first slot that runs script, or -1. A NULL script finds a free slot. */
s8 ScriptSlot_Find(const ScriptCommand* script) {
  u8 i;

  for (i = 0; i < SCRIPT_SLOT_COUNT; i++) {
    if (data_027c027c->scriptSlots[i].start == script) {
      return i;
    }
  }
  return -1;
}
