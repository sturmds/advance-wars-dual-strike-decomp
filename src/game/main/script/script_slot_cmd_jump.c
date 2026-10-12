#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: continues at the command the argument points to, with no
 * wait. Clears unk_38, which the move commands use as their started count. */
BOOL ScriptSlot_CmdJump(int slot) {
  data_027c027c->scriptSlots[slot].pc =
      (ScriptCommand*)data_027c027c->scriptSlots[slot].pc->arg;
  data_027c027c->scriptSlots[slot].wait = 0;
  data_027c027c->scriptSlots[slot].unk_38 = 0;
  return TRUE;
}
