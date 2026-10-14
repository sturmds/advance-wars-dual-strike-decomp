#include "types.h"
#include "game/save_data.h"

extern u8 gHasBackupMemory;
extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;

void Mem_FillBytesFF(void* dest, int count);
void Save_WriteToBackup(u16 slot, SaveBuffer* buffer, int size);
int Save_VerifyBackup(u16 slot, SaveBuffer* buffer, int size);

/* Frees every save slot (id 0xff, flags, part and counters 0) and writes a buffer of 0xff
 * bytes over each one, with up to four write and read-back tries per slot. Returns 1 when
 * there is no backup memory (gHasBackupMemory not 1) or a slot never verifies, else 0.
 * No direct callers found. */
int Save_EraseAll(void) {
  int try
    ;
  int i;

  if (gHasBackupMemory == 1) {
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      data_02297f3c.newId[i] = 0xff;
      data_02297f3c.id[i] = data_02297f3c.newId[i];
      data_02297f3c.flags[i] = 0;
      data_02297f3c.part[i] = 0;
      SAVE_SLOT_WRITES(i) = 0;
      data_02297fa8[i] = SAVE_SLOT_WRITES(i);
      Mem_FillBytesFF(&data_02298050, sizeof(SaveBuffer));
      for (try = 0; try < SAVE_WRITE_TRIES; try ++) {
        Save_WriteToBackup(i, &data_02298050, sizeof(SaveBuffer));
        if (Save_VerifyBackup(i, &data_02298050, sizeof(SaveBuffer))) {
          break;
        }
      }
      if (try == 4) {
        return 1;
      }
      data_02297f3c.unk_54[i] = data_02298050.endMark;
    }
    return 0;
  }
  return 1;
}
