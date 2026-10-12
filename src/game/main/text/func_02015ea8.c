#include "types.h"
#include "game/script_slot.h"

/* Callback for a script slot. Clears the slot's callback when unk_1e is 0,
 * then counts unk_1e down. */
void func_02015ea8(ScriptSlot* slot) {
  if (slot->unk_1e == 0) {
    slot->callback = NULL;
  }
  slot->unk_1e -= 1;
}
