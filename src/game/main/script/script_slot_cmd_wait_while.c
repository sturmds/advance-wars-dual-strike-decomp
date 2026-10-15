#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Script command: calls the function the argument points to with the slot.
 * While it returns nonzero the slot remains on this command and ends its run
 * for the frame; when it returns 0, moves on to the next command. */
BOOL ScriptSlot_CmdWaitWhile(int slot) {
  BOOL (*check)(ScriptSlot*) =
      (BOOL (*)(ScriptSlot*))data_027c027c->scriptSlots[slot].pc->arg;

  if (check(&data_027c027c->scriptSlots[slot])) {
    return FALSE;
  }
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
