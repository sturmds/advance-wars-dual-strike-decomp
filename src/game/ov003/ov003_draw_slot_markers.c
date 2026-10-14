#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

u16 Wireless_GetChildAids(void);
void Ov003_DrawSlotMarker(int slot);

#define STATE data_ov003_02364928

/* Draws the marker of every other slot (only with data_02160ac0->unk_7c set,
 * from the mask Wireless_GetChildAids returns) that is in state 2 and on the current
 * slot's row, then the marker of the current slot if it is in state 2. */
void Ov003_DrawSlotMarkers(void) {
  int i;

  if (data_02160ac0->unk_7c != 0) {
    for (i = 0; i < OV003_SLOT_COUNT; i++) {
      if (((Wireless_GetChildAids() | 1) & (1 << i)) &&
          STATE->slots[i].state == 2 &&
          STATE->slots[i].row == STATE->slots[STATE->currentSlot].row &&
          i != STATE->currentSlot) {
        Ov003_DrawSlotMarker(i);
      }
    }
  }
  if (STATE->slots[STATE->currentSlot].state == 2) {
    Ov003_DrawSlotMarker(STATE->currentSlot);
  }
}
