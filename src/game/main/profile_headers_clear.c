#include "types.h"
#include "game/save_progress.h"
#include "game/Unk_02290118.h"

void ProfileHeaders_Clear(void) {
  int i;
  ProfileHeader* record = data_02290a60;

  for (i = 0; i < SAVE_PROFILE_COUNT; i++, record++) {
    record->unk_00 = 0;
    record->unk_01 = 0;
    record->hardMode = 0;
    record->unk_04 = 0;
    record->unk_06 = 0;
    record->inUse = 0;
    record->unk_05 = 0;
    record->unk_08 = 0;
    record->unk_0a = 0;
    record->unk_0c = 0;
    record->unk_0e = 0;
  }
}
