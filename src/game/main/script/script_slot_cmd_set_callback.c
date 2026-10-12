#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: stores the function in the argument as the slot's
 * callback and advances. While the callback is set the slot does not run
 * the next command. */
BOOL ScriptSlot_CmdSetCallback(int slot) {
  data_027c027c->scriptSlots[slot].callback =
      (void (*)(ScriptSlot*))data_027c027c->scriptSlots[slot].pc->arg;
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
