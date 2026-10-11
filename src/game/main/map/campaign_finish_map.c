#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290118.h"
#include "game/Unk_022a7a9c.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 gCampaignBestAverage[2];

BOOL Player1Team_HasPlayerInGame(void);
int Campaign_IsHardMode(void);
int func_020d618c(void);
void CampaignRecords_Store(int mapIndex, int days, int score);
int CampaignMap_IsFinalMission(void);
void func_0210375c(void);
void func_02102c4c(void);

/* Runs when a campaign map ends. A win stores the day count and score as the
 * map's record (maps from id 224 on). When CampaignMap_IsFinalMission then returns
 * nonzero, the best average score of the current difficulty is raised if the
 * records now average higher, and a task starts; otherwise
 * func_02102c4c runs. */
void Campaign_FinishMap(void)
{
    u8 recorded = 0;
    int hard;

    if (Player1Team_HasPlayerInGame()) {
        CampaignRecords_Store(data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId - 224,
                      data_027c027c->unk_48[data_027c0284]->day, data_022a7a9c.score);
        recorded = CampaignMap_IsFinalMission();
    }
    if (recorded) {
        hard = Campaign_IsHardMode();
        if (gCampaignBestAverage[hard] < func_020d618c()) {
            hard = Campaign_IsHardMode();
            gCampaignBestAverage[hard] = func_020d618c();
        }
        data_02290118.unk_04 = 0xff;
        func_0210375c();
    } else {
        func_02102c4c();
    }
}
