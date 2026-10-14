#include "types.h"
#include "game/ov003_unit.h"

/* Ov003Slot.rowState of a slot, which holds two nibbles. Read
 * through its own base so the compiler loads its offset from the literal pool
 * as the original does. */
typedef struct SlotByte {
  u8 row : 4;  /* same as Ov003Slot.row */
  u8 mode : 4; /* meaning unknown; this function accepts 0 and 3 */
  u8 pad_01[3];
} SlotByte;

/* 1 when the current slot's mode is 0 or 3, and its row has Ov003Row.active
 * clear. Always 0 while Ov003State.unk_1df_1 is set. */
int Ov003_IsCurrentSlotRowOpen(void) {
  Ov003State* state = data_ov003_02364928;
  SlotByte* slot;

  if (state->unk_1df_1) {
    return 0;
  }
  slot = (SlotByte*)&state->slots[0].rowState + state->currentSlot;
  if (slot->mode == 3 || slot->mode == 0) {
    if (state->rows[slot->row - 1].active) {
      return 0;
    }
    return 1;
  }
  return 0;
}
