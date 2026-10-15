#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02004984(u16 arg);

/* Script command: calls a function with the command's 16-bit argument and
 * moves on to the next command. Returns FALSE, so the slot stops running
 * commands this frame. */
BOOL func_020168a8(int slot) {
  func_02004984(data_027c027c->scriptSlots[slot].pc->arg16);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
