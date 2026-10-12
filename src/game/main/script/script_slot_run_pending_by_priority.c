#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

void ScriptSlot_RunPending(int slot);

/* Runs every used slot with bit 0 of unk_12 set (see ScriptSlot_RunAll), in
 * order of the low 7 bits of their priority, lowest first. The sort is not
 * stable: slots of equal priority may run out of slot order. */
void ScriptSlot_RunPendingByPriority(void) {
  u8 order[32];
  u8 i;
  u8 j;
  u8 count;
  u8 tmp;

  for (i = 0, count = 0; i < 30; i++) {
    if (data_027c027c->scriptSlots[i].start != NULL &&
        (data_027c027c->scriptSlots[i].unk_12 & 1)) {
      order[count] = i;
      count++;
    }
  }
  if (count == 0) {
    return;
  }
  if (count != 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if ((data_027c027c->scriptSlots[order[i]].priority & 0x7f) >
            (data_027c027c->scriptSlots[order[j]].priority & 0x7f)) {
          tmp = order[i];
          order[i] = order[j];
          order[j] = tmp;
        }
      }
    }
  }
  for (i = 0; i < count; i++) {
    /* an earlier slot may have stopped this one */
    if (data_027c027c->scriptSlots[order[i]].start != NULL) {
      ScriptSlot_RunPending(order[i]);
    }
  }
}
