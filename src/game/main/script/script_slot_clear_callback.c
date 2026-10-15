#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Clears the callback of a slot, so its script advances again. */
void ScriptSlot_ClearCallback(int slot) {
  data_027c027c->scriptSlots[slot].callback = NULL;
}
