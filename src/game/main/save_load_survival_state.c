#include "types.h"
#include "game/save_progress.h"

extern Unk_022a7b80 data_022a7b80;

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */

/* Loads save record 0 into data_02291774 and restores the Survival run state
 * (data_022a7b80) and PlayRecords.unk_0d from it. Does nothing when the
 * load fails. */
void Save_LoadSurvivalState(void) {
  SaveProgressBlocks* record = &data_02291774.blocks;

  if (Save_LoadRecord(0, record)) {
    return;
  }
  data_022a7b80 = record->unk_1590;
  gPlayRecords.unk_0d = record->playRecords.unk_0d;
}
