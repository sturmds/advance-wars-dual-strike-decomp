#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/Unk_02290118.h"

int Co_GetRank(u32 co);

extern MenuState gMenuState;
extern u8 data_021687c4[];

/* Needed to match: the original adds the offset of topCos to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define TOP_COS                                \
  ((CampaignTopCo*)((u8*)&gMenuState + (u32) & \
                    ((MenuState*)0)->campaignMap.topCos))

/* Picks the three COs with the highest score (Unk_02290118.coScore) from the
 * zero-terminated list data_021687c4 into the top COs of the world map,
 * highest first, then fills in their ranks. The declaration order and the
 * for-loop forms are needed to match. */
void CampaignMap_FindTopCos(void) {
  u8* co;
  CampaignTopCo* top;
  int clearSlot;
  int rankSlot;
  int shift;
  int place;

  top = TOP_COS;
  for (clearSlot = 0; clearSlot < CAMPAIGN_TOP_CO_COUNT; clearSlot++, top++) {
    top->co = 0;
    top->rank = 0;
    top->score = 0;
  }
  for (co = data_021687c4; *co != 0; co++) {
    for (place = 0, top = TOP_COS; place < 3; place++, top++) {
      if (data_02290118.coScore[*co] > top->score) {
        for (shift = 2; shift > place; shift--) {
          gMenuState.campaignMap.topCos[shift].co =
              gMenuState.campaignMap.topCos[shift - 1].co;
          gMenuState.campaignMap.topCos[shift].score =
              gMenuState.campaignMap.topCos[shift - 1].score;
        }
        gMenuState.campaignMap.topCos[place].co = *co;
        gMenuState.campaignMap.topCos[place].score = data_02290118.coScore[*co];
        break;
      }
    }
  }
  top = TOP_COS;
  for (rankSlot = 0; rankSlot < CAMPAIGN_TOP_CO_COUNT; rankSlot++, top++) {
    if (top->co != 0) {
      top->rank = Co_GetRank(top->co);
    }
  }
}
