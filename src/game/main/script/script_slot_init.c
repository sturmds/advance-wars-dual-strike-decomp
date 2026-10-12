#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

#define SLOT_VARS(slot) (((ScriptSlotVars*)data_027c027c->scriptSlots)[slot])

/* Sets up a slot to run script from its first command with the given
 * priority (see ScriptSlot_RunPendingByPriority), and clears the rest of
 * the slot's state. */
void ScriptSlot_Init(const ScriptCommand* script, int slot, int priority) {
  data_027c027c->scriptSlots[slot].start = (ScriptCommand*)script;
  data_027c027c->scriptSlots[slot].pc = (ScriptCommand*)script;
  data_027c027c->scriptSlots[slot].wait = 0;
  data_027c027c->scriptSlots[slot].onStop = NULL;
  data_027c027c->scriptSlots[slot].unk_38 = 0;
  data_027c027c->scriptSlots[slot].priority = priority;
  data_027c027c->scriptSlots[slot].textEntry = NULL;
  data_027c027c->scriptSlots[slot].callback = NULL;
  SLOT_VARS(slot).words[0] = SLOT_VARS(slot).words[1] =
      SLOT_VARS(slot).words[2] = SLOT_VARS(slot).words[3] = -1;
  SLOT_VARS(slot).shorts[0] = SLOT_VARS(slot).shorts[1] =
      SLOT_VARS(slot).shorts[2] = SLOT_VARS(slot).shorts[3] = 0;
}
