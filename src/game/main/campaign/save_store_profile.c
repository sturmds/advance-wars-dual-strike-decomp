#include "types.h"
#include "game/campaign_map.h"
#include "game/play_records.h"
#include "game/save_progress.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

extern CampaignMapState gCampaignMapState;
extern SaveProfileBlock data_02290298[3];
extern SaveBytes30 data_022900bc[3];

int CampaignRecords_GetNextNumber(void);

/* Counterpart of Save_LoadProfile: updates the header of the live campaign
 * progress (profile, next record number unless the campaign is done (0xff),
 * day, store count, and with a day the current map in unk_0c) and copies the
 * header, the 0x180-byte block and the world map state to the profile's
 * slots. With day 0 unk_0c and unk_0e are cleared before and after. */
void Save_StoreProfile(int profile, int day) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int count;

  gPlayRecords.unk_11 = profile;
  if (day) {
    if (!data_02290118.unk_05) {
      data_02290118.unk_0e = 0;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
    data_02290118.unk_05 = 1;
    data_02290118.unk_0c = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    data_02290118.unk_0c = 0;
    data_02290118.unk_0e = 0;
  }
  data_02290118.unk_00 = profile;
  if (data_02290118.unk_04 != 0xff) {
    data_02290118.unk_04 = CampaignRecords_GetNextNumber();
  }
  data_02290118.unk_06 = day;
  count = data_02290118.unk_03;
  if (count < 0xff) {
    count++;
  }
  data_02290118.unk_03 = count;
  data_02290a60[profile] = *(ProfileHeader*)&data_02290118;
  data_02290298[profile] = *(SaveProfileBlock*)&data_02290118;
  data_022900bc[profile] = *(SaveBytes30*)&gCampaignMapState;
  if (!day) {
    data_02290118.unk_0c = 0;
    data_02290118.unk_0e = 0;
  }
}
