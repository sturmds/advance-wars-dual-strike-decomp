#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

/* Frees both show task slots. */
void UnitShowTask_ClearSlots(void) {
  u16 i;

  for (i = 0; i < UNIT_SHOW_SLOT_COUNT; i++) {
    data_027c027c->unk_58[i] = 0;
  }
}
