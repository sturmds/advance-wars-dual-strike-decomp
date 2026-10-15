#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02004dfc(u16 arg);

BOOL func_02016bb4(int slot) {
  func_02004dfc(data_027c027c->scriptSlots[slot].pc->arg16);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
