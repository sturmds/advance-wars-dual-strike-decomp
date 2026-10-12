#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* True when a used slot still has bit 0 of unk_12 set. ScriptSlot_RunAll sets
 * that bit on every used slot and ScriptSlot_RunPending clears it when it runs the slot. */
BOOL ScriptSlot_AnyPending(void) {
  u8 i;

  for (i = 0; i < 30; i++) {
    if (data_027c027c->scriptSlots[i].start != NULL &&
        (data_027c027c->scriptSlots[i].unk_12 & 1)) {
      return TRUE;
    }
  }
  return FALSE;
}
