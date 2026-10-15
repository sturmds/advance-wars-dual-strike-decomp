#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void ScriptSlot_StopSlot(u32 arg);

BOOL func_02016d24(int slot) {
  ScriptSlot_StopSlot(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
