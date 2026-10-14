#include "types.h"
#include "game/play_records.h"
#include "game/save_progress.h"

extern Unk_0229e148 data_0229e148;

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */

/* Loads save record 0 into data_02291774 and restores the combat setup
 * (data_0229e148) from it. Then lowers the two PlayRecords values that
 * PlayRecords_CapPoints and func_02017c88 limit to the record's. Does nothing when
 * the load fails. */
void Save_LoadCombatSetup(void) {
  SaveProgressBlocks* record = &data_02291774.blocks;

  if (Save_LoadRecord(0, record)) {
    return;
  }
  data_0229e148 = record->unk_15a4;
  PlayRecords_CapPoints(record->playRecords.points);
  func_02017c88(record->playRecords.unk_04);
}
