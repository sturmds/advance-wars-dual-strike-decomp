#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_020174fc(int slot, u16 arg);

/* Script command: calls a function with the slot and the command's 16-bit argument,
 * moves on to the next command and keeps running the slot. */
BOOL func_02015efc(int slot) {
  func_020174fc(slot, data_027c027c->scriptSlots[slot].pc->arg16);
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
