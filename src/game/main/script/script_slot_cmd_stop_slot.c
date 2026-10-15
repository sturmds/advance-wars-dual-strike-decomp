#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void ScriptSlot_Stop(int slot);

/* Script command: stops the slot whose number is in the 16-bit argument,
 * then goes on. */
BOOL ScriptSlot_CmdStopSlot(int slot) {
  ScriptSlot_Stop((s16)data_027c027c->scriptSlots[slot].pc->arg16);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
