#include "types.h"
#include "game/save_data.h"

extern SaveSlotTable data_02297f3c;

u16 Save_FindNewestRecord(int recordId);

/* Returns 1 when recordId cannot be used: it is 0xff, Save_FindNewestRecord returns
 * 0xffff, a slot without the mark holds it, or a marked slot holds another
 * id. Returns 0 otherwise. The meaning of the checks is not clear yet. */
int func_0201fe28(int recordId) {
  int i;

  if (recordId == 0xff) {
    return 1;
  }
  if (Save_FindNewestRecord(recordId) == 0xffff) {
    return 1;
  }
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (!(data_02297f3c.flags[i] & SAVE_SLOT_MARK)) {
      if (data_02297f3c.id[i] == recordId) {
        return 1;
      }
    } else if (data_02297f3c.id[i] != recordId) {
      return 1;
    }
  }
  return 0;
}
