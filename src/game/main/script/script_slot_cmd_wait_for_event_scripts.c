#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

BOOL EventScript_IsRunning(void);

/* Script command: waits until no event script is running, then advances. */
BOOL ScriptSlot_CmdWaitForEventScripts(int slot) {
  if (EventScript_IsRunning()) {
    return FALSE;
  }
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
