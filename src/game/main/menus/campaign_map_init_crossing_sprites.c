#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/unit_show_task.h"

ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);
s32 Rng_Range(s32 min, s32 max);

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define WORK_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->campaignMap.field)

/* Makes the two sprites at CampaignMapWork.sprites (frames 0 and 1 of the
 * unk_244 file) and gives the task random waits of 0 to 59 frames before
 * CampaignMap_UpdateCrossingSprites moves them across the map. */
void CampaignMap_InitCrossingSprites(CampaignMapTask* task) {
  gMenuState.campaignMap.sprites[0] =
      SpriteAnim_Create(WORK_FIELD(unk_244), 1, 2);
  SpriteAnim_SetAnimation(gMenuState.campaignMap.sprites[0], 0);
  gMenuState.campaignMap.sprites[0]->unk_24 = 0x97c;
  gMenuState.campaignMap.sprites[1] =
      SpriteAnim_Create(WORK_FIELD(unk_244), 1, 2);
  SpriteAnim_SetAnimation(gMenuState.campaignMap.sprites[1], 1);
  gMenuState.campaignMap.sprites[1]->unk_24 = 0x97c;
  task->unk_64 = 2;
  task->unk_66 = 2;
  task->sprite0Wait = Rng_Range(0, 60);
  task->sprite1Wait = Rng_Range(0, 60);
  task->step++;
}
