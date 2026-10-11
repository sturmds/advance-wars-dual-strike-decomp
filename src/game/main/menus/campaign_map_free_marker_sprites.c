#include "types.h"
#include "game/menu_state.h"

void SpriteAnim_Free(void *handle);

extern MenuState gMenuState;

void CampaignMap_FreeMarkerSprites(void)
{
    SpriteAnim_Free(gMenuState.campaignMap.sprites[0]);
    SpriteAnim_Free(gMenuState.campaignMap.sprites[1]);
}
