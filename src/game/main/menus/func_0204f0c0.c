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

/* Starts the sprite task at the current mission, 8 pixels right and up of its screen position (animation 0). Names of the sprite and the mission index (gCampaignMapState.playedMission) are guesses. */
void func_0204f0c0(void) {
  Unk_027c0280* gfx;
  int bg;
  int mission;

  mission = gCampaignMapState.playedMission;
  gfx = data_027c0280;
  bg = gfx->layer3Bg;
  SpriteAnimTask_Start(
      WORK_FIELD(unk_0e4),
      (gCampaignMissions[mission].x - gfx->bgScrollX[bg] + 8) & 0x1ff,
      (gCampaignMissions[mission].y - gfx->bgScrollY[bg] - 8) & 0xff, 0x218a, 0,
      0);
}
