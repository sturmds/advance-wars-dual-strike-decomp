#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"

struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);
BOOL func_02021b34(void);
int Sound_PlaySe(int id);
void Task_ResumeByScript(const void* script);
void Task_ResumeScript(CampaignMapTask* task);

extern CampaignMapState gCampaignMapState;
extern Unk_027c0280* data_027c0280;
extern MenuState gMenuState;
extern CampaignMission gCampaignMissions[];
extern u8 data_0215b420[];

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define WORK_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->campaignMap.field)

void func_0204e808(CampaignMapTask* task) {
  Unk_027c0280* gfx;
  int bg;
  int mission;

  if (task->step == 0) {
    mission = gCampaignMapState.mission;
    gfx = data_027c0280;
    bg = gfx->layer3Bg;
    SpriteAnimTask_Start(
        WORK_FIELD(unk_020),
        (gCampaignMissions[mission].x - gfx->bgScrollX[bg] + 8) & 0x1ff,
        (gCampaignMissions[mission].y - gfx->bgScrollY[bg] + 16) & 0xff, 0x904,
        0, 3);
    Sound_PlaySe(0x57);
  }
  if (func_02021b34()) {
    task->step++;
    return;
  }
  task->step = 0;
  Task_ResumeByScript(data_0215b420);
  Task_ResumeScript(task);
}
