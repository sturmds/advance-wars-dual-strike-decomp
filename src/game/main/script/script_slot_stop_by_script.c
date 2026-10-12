#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

void ScriptSlot_Stop(int slot);

/* Stops the script slot that runs script (ScriptSlot_Stop clears it and calls
 * its callback) and returns its index, or -1 when none runs it. */
s8 ScriptSlot_StopByScript(const ScriptCommand* script) {
  u8 i;

  for (i = 0; i < 30; i++) {
    if (data_027c027c->scriptSlots[i].start == script) {
      ScriptSlot_Stop(i);
      return i;
    }
  }
  return -1;
}
