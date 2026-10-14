#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

BOOL ScriptSlot_AnyPending(void);
void ScriptSlot_RunPendingByPriority(void);

/* Flags every used slot, then runs the flagged ones in priority order
 * (ScriptSlot_RunPendingByPriority) until no flagged slot is left. */
void ScriptSlot_RunAll(void) {
  u8 i;

  for (i = 0; i < SCRIPT_SLOT_COUNT; i++) {
    if (data_027c027c->scriptSlots[i].start != NULL) {
      data_027c027c->scriptSlots[i].unk_12 |= 1;
    }
  }
  do {
    ScriptSlot_RunPendingByPriority();
  } while (ScriptSlot_AnyPending());
}
