#include "types.h"
#include "game/save_data.h"

int Save_ReadSlot(u16 slot);
int func_0201fe28(int recordId);

extern int data_02297f38;
extern u8* gSaveBuffer;
extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;

/* Loads the save record recordId (a guess: func_0201fe28 and Save_ReadSlot
 * look like a record check and a read from the save buffer). For each slot
 * that holds the record, the data in the save buffer is copied to
 * gSaveBuffer + destOffset and its length is added to data_02297f38.
 * Record 0 also replaces the slot ids with the ones the buffer holds.
 * Returns 1 on failure.
 *
 * Needed to match: n holds the result of func_0201fe28 and later counts the
 * bytes and slots of the two inner loops. */
int Save_LoadRecord(int recordId) {
  int n;
  u8 savedMarks[SAVE_SLOT_COUNT];
  int i;

  data_02297f38 = 0;
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    savedMarks[i] = data_02297f3c.flags[i] & SAVE_SLOT_MARK;
  }
  n = func_0201fe28(recordId);
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    data_02297f3c.flags[i] =
        savedMarks[i] | (data_02297f3c.flags[i] & ~SAVE_SLOT_MARK);
  }
  if (n) {
    return 1;
  }

  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (recordId == data_02297f3c.id[i]) {
      if (Save_ReadSlot(i)) {
        return 1;
      }
      data_02297f38 += data_02298050.dataLength;
      for (n = 0; n < data_02298050.dataLength; n++) {
        (gSaveBuffer + data_02298050.destOffset)[n] = data_02298050.data[n];
      }
      if ((data_02298050.part & 0xf0) == 0 && recordId == 0) {
        for (n = 0; n < SAVE_SLOT_COUNT; n++) {
          data_02297f3c.newId[n] = data_02298050.slotIds[n];
        }
      }
    }
  }
  if (recordId == 0) {
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      data_02297f3c.id[i] = data_02297f3c.newId[i];
    }
  }
  return 0;
}
