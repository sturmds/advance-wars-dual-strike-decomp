#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: stays on this command while any of the 30 slots runs
 * the script the argument points to, then advances. */
BOOL ScriptSlot_CmdWaitForScript(int slot) {
  u8 i;

  for (i = 0; i < SCRIPT_SLOT_COUNT; i++) {
    if (data_027c027c->scriptSlots[i].start ==
        (ScriptCommand*)data_027c027c->scriptSlots[slot].pc->arg) {
      return FALSE;
    }
  }
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
