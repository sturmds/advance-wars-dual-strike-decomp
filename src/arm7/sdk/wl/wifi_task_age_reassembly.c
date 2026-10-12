#include "sdk/wl.h"

// Task 17: counts down the fragment reassembly slots in use and frees the
// buffer of each one whose count reaches 0, giving up on that frame.
void WifiTask_AgeReassembly(void) {
  WlDefragSlot* slots = WIFI_DRIVER_STATE->defragSlots;
  u32 i;

  for (i = 0; i < WIFI_REASSEMBLY_SLOTS; i++) {
    WlDefragSlot* slot = &slots[i];

    if (slot->ticks != 0) {
      slot->ticks--;
      if (slot->ticks == 0) {
        WifiBuffer_Release(&WIFI_DRIVER_STATE->unk_15c, &slot->buffer->header);
      }
    }
  }
}
