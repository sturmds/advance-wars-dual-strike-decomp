#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void ScriptSlot_Stop(int slot);

/* Stops the given slot (a pointer into the slot array) and returns its index, or -1
 * when target is not one of the 30 slots. */
s8 ScriptSlot_StopSlot(ScriptSlot* target) {
  u8 i;

  for (i = 0; i < SCRIPT_SLOT_COUNT; i++) {
    if (&data_027c027c->scriptSlots[i] == target) {
      ScriptSlot_Stop(i);
      return i;
    }
  }
  return -1;
}
