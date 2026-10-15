#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void EventScript_Start(u32 script);

BOOL func_02016e1c(int slot) {
  EventScript_Start(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
