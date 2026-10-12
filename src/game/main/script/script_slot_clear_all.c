#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Frees all 30 script slots (used when a screen is reset). */
void ScriptSlot_ClearAll(void) {
  u8 i;

  for (i = 0; i < 30; i++) {
    data_027c027c->scriptSlots[i].start = NULL;
  }
  *data_027c027c->unk_40 = 0;
}
