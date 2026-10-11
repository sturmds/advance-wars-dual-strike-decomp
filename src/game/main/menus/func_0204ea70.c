#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);
void Task_ResumeByScript(const void *script);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern Unk_027c0280 *data_027c0280;
extern u8 data_0215b420[];

/* Script step: sets the scroll of the two map layers (the BGs of layers 3 and
 * 2) so the current mission is centred, then resumes the script by
 * data_0215b420. */
void func_0204ea70(void)
{
    int bgA;
    int bgB;

    bgA = data_027c0280->layer3Bg;
    bgB = data_027c0280->layer2Bg;
    data_027c0280->bgScrollX[bgA] = CampaignMap_CenterScrollX(gCampaignMissions[gCampaignMapState.mission].x);
    data_027c0280->bgScrollX[bgB] = data_027c0280->bgScrollX[bgA];
    bgA = data_027c0280->layer3Bg;
    bgB = data_027c0280->layer2Bg;
    data_027c0280->bgScrollY[bgA] = func_0204ec34(gCampaignMissions[gCampaignMapState.mission].y);
    data_027c0280->bgScrollY[bgB] = data_027c0280->bgScrollY[bgA];
    Task_ResumeByScript(data_0215b420);
}
