#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

s8 ScriptSlot_StopByScript(u32 script);

BOOL func_02016cd0(int slot) {
  ScriptSlot_StopByScript(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
