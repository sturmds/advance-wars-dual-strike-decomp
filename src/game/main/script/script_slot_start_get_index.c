#include "types.h"
#include "game/script_slot.h"

s8 ScriptSlot_Find(const ScriptCommand* script);
void ScriptSlot_StartInSlotNoFlags(const ScriptCommand* script, s16 slot,
                                   int arg);

/* Like ScriptSlot_Start, but starts through ScriptSlot_StartInSlotNoFlags and returns
 * the slot index, or -1 when all 30 are in use. */
s8 ScriptSlot_StartGetIndex(const ScriptCommand* script, int arg) {
  s8 slot = ScriptSlot_Find(NULL);

  if (slot != -1) {
    ScriptSlot_StartInSlotNoFlags(script, slot, arg);
  }
  return slot;
}
