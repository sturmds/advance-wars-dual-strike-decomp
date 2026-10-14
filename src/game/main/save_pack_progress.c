#include "types.h"
#include "game/save_progress.h"

extern SaveProfileBlock data_02290298[];
extern SaveBytes30 data_022900bc[];
extern Unk_022a7b80 data_022a7b80;
extern Unk_0229e148 data_0229e148;

/* Copies the live data into the blocks of save record 0 at record. Returns
 * the record size. */
int Save_PackProgress(SaveProgressBlocks* record) {
  int i;

  record->playRecords = gPlayRecords;
  for (i = 0; i < SAVE_PROFILE_COUNT; i++) {
    record->profileBlocks[i] = data_02290298[i];
    record->unk_1534[i] = data_022900bc[i];
  }
  record->gameFlagsMid = gGameFlagsMid;
  record->unk_1590 = data_022a7b80;
  record->unk_15a4 = data_0229e148;
  record->linkReactionTexts = data_0229da8c;
  return sizeof(SaveProgressBlocks);
}
