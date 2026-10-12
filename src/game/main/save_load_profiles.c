#include "types.h"
#include "game/Unk_02290118.h"
#include "game/play_records.h"
#include "game/save_progress.h"

/* Where the three profile blocks start in the save record. This is 16 bytes
 * before SaveProgress.profileSets. */
#define SAVE_PROFILE_BLOCKS_OFFSET 0x10b4

extern SaveProfileBlock data_02290298[3];
extern SaveBytes30 data_022900bc[3];
extern Unk_02290118 data_02290118;

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
void Save_LoadProfile(int profile);

/* Loads save record 0 into data_02291774 and copies the data of its three
 * profiles to the live copies, then runs Save_LoadProfile for the current profile. */
void Save_LoadProfiles(void) {
  SaveProgress* progress = &data_02291774;
  int i;

  if (Save_LoadRecord(0, progress)) {
    return;
  }
  for (i = 0; i < 3; i++) {
    data_02290298[i] =
        *(SaveProfileBlock*)((u8*)&((SaveProfileBlock*)progress)[i] +
                             SAVE_PROFILE_BLOCKS_OFFSET);
    gPlayRecords.unk_348[i] = progress->unk_3a0[i];
    data_022900bc[i] = progress->unk_1534[i];
  }
  Save_LoadProfile(data_02290118.unk_00);
}
