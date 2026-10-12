#include "types.h"
#include "game/Unk_02290118.h"
#include "game/unlocks.h"

void CampaignMap_ResetState(void);

/* Clears the campaign map state, the scores of CO ids 0 to 27 and game flags
 * 95 to 158. */
void Campaign_ResetProgress(void) {
  int i;
  int flag;

  CampaignMap_ResetState();
  data_02290118.unk_8c &= 0xfff000ff;
  for (i = 0; i < 28; i++) {
    data_02290118.coScore[i] = 0;
  }
  for (flag = 95; flag < 159; flag++) {
    GameFlag_Set(flag, 0);
  }
}
