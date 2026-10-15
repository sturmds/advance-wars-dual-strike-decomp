#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

/* Marks every slot of every icon bank as unused (tile 0xFFFF). */
void IconBanks_ClearSlots(void) {
  int bank;
  int slot;

  for (bank = 0; bank < ICON_BANK_COUNT; bank++) {
    for (slot = 0; slot < ICON_BANK_SLOTS; slot++) {
      data_027c0280->iconBanks[bank].slots[slot].tile = 0xFFFF;
    }
  }
}
