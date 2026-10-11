#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/unit_show_task.h"

ShowSprite *func_02022478(void *file, int a, int b);
void func_02021f44(ShowSprite *sprite, int frame);
s32 Rng_Range(s32 min, s32 max);

extern MenuState gMenuState;

/* Needed to match: the original adds the field offset to the address of
 * gMenuState at run time instead of folding it into the relocation. */
#define WORK_FIELD(field) ((u8 *)&gMenuState + (u32)&((MenuState *)0)->campaignMap.field)

/* Makes the two sprites at CampaignMapWork.sprites (frames 0 and 1 of the
 * unk_244 file) and gives the task random waits of 0 to 59 frames. The name
 * is a guess: nothing shows what the sprites look like. */
void CampaignMap_InitMarkerSprites(CampaignMapTask *task)
{
    gMenuState.campaignMap.sprites[0] = func_02022478(WORK_FIELD(unk_244), 1, 2);
    func_02021f44(gMenuState.campaignMap.sprites[0], 0);
    gMenuState.campaignMap.sprites[0]->unk_24 = 0x97c;
    gMenuState.campaignMap.sprites[1] = func_02022478(WORK_FIELD(unk_244), 1, 2);
    func_02021f44(gMenuState.campaignMap.sprites[1], 1);
    gMenuState.campaignMap.sprites[1]->unk_24 = 0x97c;
    task->unk_64 = 2;
    task->unk_66 = 2;
    task->unk_58 = Rng_Range(0, 60);
    task->unk_5c = Rng_Range(0, 60);
    task->step++;
}
