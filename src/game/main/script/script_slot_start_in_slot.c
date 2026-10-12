#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

void ScriptSlot_Run(int slot);
void ScriptSlot_Init(const ScriptCommand* script, int slot, int arg);

/* Sets up slot to run script with arg (ScriptSlot_Init), runs its first commands
 * at once, and returns the slot. scriptSlotCurrent holds the slot number during
 * that run. */
s8 ScriptSlot_StartInSlot(const ScriptCommand* script, u8 slot, int arg) {
  s16 saved = *data_027c027c->scriptSlotCurrent;

  *data_027c027c->scriptSlotCurrent = slot;
  ScriptSlot_Init(script, slot, arg);
  data_027c027c->scriptSlots[slot].unk_12 = 4;
  ScriptSlot_Run(slot);
  *data_027c027c->scriptSlotCurrent = saved;
  return slot;
}
