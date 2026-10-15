#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Script command: when the flag at unk_40 is set, clears it and continues at
 * the command the argument points to; otherwise moves on to the next
 * command. Runs the next command this frame either way. */
BOOL ScriptSlot_CmdJumpIfFlag(int slot) {
  if (*data_027c027c->unk_40 != 0) {
    *data_027c027c->unk_40 = 0;
    data_027c027c->scriptSlots[slot].pc =
        (ScriptCommand*)data_027c027c->scriptSlots[slot].pc->arg;
    return TRUE;
  }
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
