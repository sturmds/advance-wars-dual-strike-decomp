#include "types.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

/* Needed to match: the slot byte at 0x139 of Ov003State (Ov003Slot.row and
 * state), read as a nibble pair at a 0x139 base. */
typedef struct SlotNibbles {
  u8 low : 4;
  u8 high : 4;
  u8 pad[3];
} SlotNibbles;

#define SLOT_NIBBLES(state) ((SlotNibbles*)((u8*)(state) + 0x139))

u16 Wireless_GetChildAids(void);

/* Does nothing while data_02160abc->unk_0a is set. Without the link flag, sets
 * the state (high nibble) of the current slot to 3. With it, sets state 3 on
 * each linked slot whose row is 0 when the current slot's row is 1, or whose
 * row is not 0 when the current row is 0. */
void Ov003_SetSlotsScrolling(void) {
  int i;
  Ov003State* state;
  SlotNibbles* slot;

  if (data_02160abc->unk_0a != 0) {
    return;
  }
  if (data_02160ac0->unk_7c == 0) {
    SLOT_NIBBLES(data_ov003_02364928)
    [data_ov003_02364928->currentSlot].high = 3;
    return;
  }
  for (i = 0; i < 8; i++) {
    if ((Wireless_GetChildAids() | 1) & (1 << i)) {
      state = data_ov003_02364928;
      slot = &SLOT_NIBBLES(state)[i];
      if ((slot->low == 0 ? 1 : 0) ==
          SLOT_NIBBLES(state)[state->currentSlot].low) {
        slot->high = 3;
      }
    }
  }
}
