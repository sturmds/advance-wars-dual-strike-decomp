#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: calls the function the argument points to with the slot.
 * Until it returns exactly 1 the slot remains on this command and ends its run
 * for the frame; then it moves on to the next command. */
BOOL ScriptSlot_CmdWaitUntilOne(int slot) {
  BOOL (*check)(ScriptSlot*) =
      (BOOL (*)(ScriptSlot*))data_027c027c->scriptSlots[slot].pc->arg;

  if (check(&data_027c027c->scriptSlots[slot]) != TRUE) {
    return FALSE;
  }
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
