#include "types.h"
#include "game/save_data.h"

extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;

int Save_ReadFromBackup(int slot, int arg1, SaveBuffer* buffer, int size);
int Save_CheckBuffer(void);

/* Reads save slot slot into the save buffer and checks it. Sets the slot's
 * flag bit 2 and returns 4 when the check fails; otherwise clears bits 0 and
 * 2 and returns 0. */
int Save_ReadSlot(int slot) {
  Save_ReadFromBackup(slot, 0, &data_02298050, 0x3000);
  if (Save_CheckBuffer()) {
    data_02297f3c.flags[slot] |= 4;
    return 4;
  }
  data_02297f3c.flags[slot] &= ~5;
  return 0;
}
