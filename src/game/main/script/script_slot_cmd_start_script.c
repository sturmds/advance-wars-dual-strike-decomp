#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

s8 ScriptSlot_StartGetIndex(const ScriptCommand* script, int arg);

/* Script command: starts the script in the argument in a free slot, passing
 * the low byte of the 16-bit argument, then goes on. */
BOOL ScriptSlot_CmdStartScript(int slot) {
  ScriptSlot_StartGetIndex(
      (ScriptCommand*)data_027c027c->scriptSlots[slot].pc->arg,
      data_027c027c->scriptSlots[slot].pc->arg16 & 0xff);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
