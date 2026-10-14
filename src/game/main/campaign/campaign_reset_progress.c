#include "types.h"
#include "game/Unk_02290118.h"
#include "game/unlocks.h"
#include "game/co_data.h"

void CampaignMap_ResetState(void);

/* Clears the campaign map state, the scores of CO ids 0 to 27 and game flags
 * 95 to 158. */
void Campaign_ResetProgress(void) {
  int i;
  int flag;

  CampaignMap_ResetState();
  data_02290118.unk_8c &= 0xfff000ff;
  for (i = 0; i < CO_COUNT; i++) {
    data_02290118.coScore[i] = 0;
  }
  for (flag = GAME_FLAG_CAMPAIGN_FIRST; flag < GAME_FLAG_CAMPAIGN_END; flag++) {
    GameFlag_Set(flag, 0);
  }
}
