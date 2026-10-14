#include "types.h"
#include "game/save_progress.h"

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
int Save_UnpackProgress(SaveProgressBlocks* record);

/* Loads save record 0 into data_02291774 and unpacks it into the live data.
 * Does nothing when the load fails. */
void Save_LoadAndUnpackProgress(void) {
  if (Save_LoadRecord(0, &data_02291774)) {
    return;
  }
  Save_UnpackProgress(&data_02291774.blocks);
}
