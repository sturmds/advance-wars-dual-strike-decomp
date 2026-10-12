#include "types.h"
#include "game/campaign_map.h"
#include "game/play_records.h"
#include "game/save_progress.h"
#include "game/Unk_02290118.h"

extern CampaignMapState gCampaignMapState;

int Save_LoadRecord(int recordId, void* buffer);
void Co_SetAvailable(int co, int value);
void PlayRecords_CapAtSnapshot(void);

/* Loads save record 0 into data_02291774, then copies from it the CO
 * availability flags, a few play record fields and the data of one profile:
 * its 0x180-byte block into data_02290118, its 16-byte header over the start
 * of that block, and its world map state. */
void Save_LoadProfile(int profile) {
  SaveProgressBlocks* save = (SaveProgressBlocks*)&data_02291774;
  int co;

  if (Save_LoadRecord(0, save)) {
    return;
  }
  for (co = 1; co < CO_COUNT; co++) {
    Co_SetAvailable(co, save->playRecords.coRecords[co].available);
  }
  gPlayRecords.unk_388 = save->playRecords.unk_388;
  gPlayRecords.unk_38c = save->playRecords.unk_38c;
  *(SaveProfileBlock*)&data_02290118 = save->profileBlocks[profile];
  *(SaveHalfwords16*)&data_02290118 = save->playRecords.unk_348[profile];
  *(SaveBytes30*)&gCampaignMapState = save->unk_1534[profile];
  PlayRecords_CapAtSnapshot();
  gPlayRecords.unk_08 = save->playRecords.unk_08;
  gPlayRecords.unk_09 = save->playRecords.unk_09;
  gPlayRecords.unk_0a = save->playRecords.unk_0a;
}
