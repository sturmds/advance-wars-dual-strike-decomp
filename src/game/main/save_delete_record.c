#include "types.h"
#include "game/save_data.h"

extern u8* gSaveBuffer;
extern int (*gSavePackFunc)(u8* dest);
extern SaveSlotTable data_02297f3c;

int Save_WriteRecord(int recordId, u8* unused, int size);

/* Deletes save record recordId: frees every slot that holds it, then writes record 0 again so
 * the change is saved. Record 0 itself cannot be deleted; the function returns 1 for it. */
int Save_DeleteRecord(int recordId) {
  int size;
  int i;

  if (recordId != 0) {
    size = gSavePackFunc(gSaveBuffer);
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      if (data_02297f3c.id[i] == recordId) {
        data_02297f3c.newId[i] = 0xff;
        data_02297f3c.id[i] = data_02297f3c.newId[i];
      }
    }
    return Save_WriteRecord(0, gSaveBuffer, size);
  }
  return 1;
}
