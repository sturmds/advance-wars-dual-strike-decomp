#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: waits for the number of frames in the 16-bit argument
 * before the next command, ending the slot's run for this frame. */
BOOL ScriptSlot_CmdWait(int slot) {
  data_027c027c->scriptSlots[slot].wait =
      data_027c027c->scriptSlots[slot].pc->arg16;
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
