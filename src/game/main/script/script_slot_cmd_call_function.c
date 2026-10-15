#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Script command: calls the function the argument holds with the slot, then goes on.
 * Returns whether the slot still has a script, since the function may
 * have stopped it. */
u8 ScriptSlot_CmdCallFunction(int slot) {
  ((void (*)(ScriptSlot*))data_027c027c->scriptSlots[slot].pc->arg)(
      &data_027c027c->scriptSlots[slot]);
  data_027c027c->scriptSlots[slot].pc++;
  return data_027c027c->scriptSlots[slot].start != NULL;
}
