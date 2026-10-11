#include "types.h"
#include "game/menu_state.h"

void func_0202244c(void *handle);

extern MenuState gMenuState;

void CampaignMap_FreeMarkerSprites(void)
{
    func_0202244c(gMenuState.campaignMap.sprites[0]);
    func_0202244c(gMenuState.campaignMap.sprites[1]);
}
