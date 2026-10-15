#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02017500(int slot, int arg);

BOOL func_02015f50(int slot) {
  func_02017500(slot, data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
