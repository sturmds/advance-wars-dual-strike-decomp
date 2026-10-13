#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02290118.h"

BOOL Co_IsAvailable(int co);
void Co_SetAvailable(int co, int value);
int Co_GetRank(u32 co);
int func_0210391c(void);

extern u8 data_021687c4[];
CampaignTopCo gCampaignUnlockedCos[4];

/* A step of the data_0216d8ec task, which Campaign_ContinueProfile0-2 start
 * when the profile's unk_04 is 0xff. Unless data_02290118.unk_08 is set,
 * picks the three COs of data_021687c4 that are not available yet with the
 * highest score (Unk_02290118.coScore), highest first, stores them with their
 * rank in gCampaignUnlockedCos (read back by Campaign_GetUnlockedCos) and makes them
 * available. Same sort as CampaignMap_FindTopCos. */
void Campaign_UnlockTopCos(void) {
  u8* co;
  CampaignTopCo* top;
  int clearSlot;
  int unlockSlot;
  int shift;
  int place;

  top = gCampaignUnlockedCos;
  for (clearSlot = 0; clearSlot < 4; clearSlot++, top++) {
    top->co = 0;
    top->rank = 0;
    top->score = 0;
  }
  if (func_0210391c()) {
    return;
  }
  for (co = data_021687c4; *co != 0; co++) {
    if (Co_IsAvailable(*co)) {
      continue;
    }
    for (place = 0, top = gCampaignUnlockedCos; place < 3; place++, top++) {
      if (data_02290118.coScore[*co] > top->score) {
        for (shift = 2; shift > place; shift--) {
          gCampaignUnlockedCos[shift].co = gCampaignUnlockedCos[shift - 1].co;
          gCampaignUnlockedCos[shift].score =
              gCampaignUnlockedCos[shift - 1].score;
        }
        gCampaignUnlockedCos[place].co = *co;
        gCampaignUnlockedCos[place].score = data_02290118.coScore[*co];
        break;
      }
    }
  }
  top = gCampaignUnlockedCos;
  for (unlockSlot = 0; unlockSlot < 3; unlockSlot++, top++) {
    if (top->co != 0) {
      top->rank = Co_GetRank(top->co);
      Co_SetAvailable(top->co, 1);
    }
  }
}
