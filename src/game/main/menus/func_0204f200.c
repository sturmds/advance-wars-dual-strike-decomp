#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"

struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern Unk_027c0280* data_027c0280;
extern MenuState gMenuState;

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define WORK_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->campaignMap.field)

/* As func_0204f0c0, 20 pixels right, animation 1. */
void func_0204f200(void) {
  /* Needed to match: through a volatile pointer the scroll X load comes
   * before the mission X load, as in the original. */
  volatile Unk_027c0280* gfx;
  int bg;
  int mission;

  mission = gCampaignMapState.playedMission;
  gfx = data_027c0280;
  bg = gfx->layer3Bg;
  SpriteAnimTask_Start(
      WORK_FIELD(unk_0e4),
      (gCampaignMissions[mission].x - gfx->bgScrollX[bg] + 20) & 0x1ff,
      (gCampaignMissions[mission].y - gfx->bgScrollY[bg]) & 0xff, 0x218a, 1, 0);
}
