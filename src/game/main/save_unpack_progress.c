#include "types.h"
#include "game/save_progress.h"
#include "game/visual_mode.h"

extern SaveGameFlagsMid gGameFlagsMid;
extern SaveProfileBlock data_02290298[3];
extern SaveBytes30 data_022900bc[3];
extern Unk_022a7b80 data_022a7b80;
extern Unk_0229e148 data_0229e148;
extern SaveLinkReactionTexts data_0229da8c;

void func_0210394c(void);

/* Copies the blocks of save record 0 (already read into record) to the live
 * data, resets two out-of-range settings to 1, then calls func_0210394c.
 * Returns the record size. */
int Save_UnpackProgress(SaveProgressBlocks* record) {
  int i;

  gPlayRecords = record->playRecords;
  for (i = 0; i < 3; i++) {
    data_02290298[i] = record->profileBlocks[i];
    data_022900bc[i] = record->unk_1534[i];
  }
  if (gPlayRecords.visualMode > VISUAL_D) {
    gPlayRecords.visualMode = VISUAL_A;
  }
  if (gPlayRecords.unk_0e > 1) {
    gPlayRecords.unk_0e = 1;
  }
  gGameFlagsMid = record->gameFlagsMid;
  data_022a7b80 = record->unk_1590;
  data_0229e148 = record->unk_15a4;
  data_0229da8c = record->linkReactionTexts;
  func_0210394c();
  return sizeof(SaveProgressBlocks);
}
