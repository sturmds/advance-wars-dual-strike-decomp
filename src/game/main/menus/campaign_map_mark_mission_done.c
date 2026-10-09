#include "types.h"
#include "game/campaign_map.h"

void func_0204d708(void);

extern CampaignMapState gCampaignMapState;
extern u8 data_0229c74a[];

/* Sets bit 0 of the flags of the mission just played (unk_00), then picks
 * the next mission (func_0204d708 writes CampaignMapState.mission). */
void CampaignMap_MarkMissionDone(void)
{
    data_0229c74a[gCampaignMapState.unk_00] |= 1;
    func_0204d708();
}
