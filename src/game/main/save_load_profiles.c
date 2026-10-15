#include "types.h"
#include "game/Unk_02290118.h"
#include "game/play_records.h"
#include "game/save_progress.h"

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
void Save_LoadProfile(int profile);

/* Loads save record 0 into data_02291774 and copies the data of its three
 * profiles to the live copies, then runs Save_LoadProfile for the current profile. */
void Save_LoadProfiles(void) {
  SaveProgressBlocks* progress = &data_02291774.blocks;
  int i;

  if (Save_LoadRecord(0, progress)) {
    return;
  }
  for (i = 0; i < SAVE_PROFILE_COUNT; i++) {
    data_02290298[i] = progress->profileBlocks[i];
    gPlayRecords.unk_348[i] = progress->playRecords.unk_348[i];
    data_022900bc[i] = progress->unk_1534[i];
  }
  Save_LoadProfile(data_02290118.unk_00);
}
