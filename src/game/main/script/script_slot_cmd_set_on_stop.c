#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Script command: sets the slot's onStop handler to the function in the
 * argument, then goes on. */
BOOL ScriptSlot_CmdSetOnStop(int slot) {
  data_027c027c->scriptSlots[slot].onStop =
      (void (*)(ScriptSlot*))data_027c027c->scriptSlots[slot].pc->arg;
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
