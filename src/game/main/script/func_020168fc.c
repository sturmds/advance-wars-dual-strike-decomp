#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_020049cc(u16 arg);

BOOL func_020168fc(int slot) {
  func_020049cc(data_027c027c->scriptSlots[slot].pc->arg16);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
