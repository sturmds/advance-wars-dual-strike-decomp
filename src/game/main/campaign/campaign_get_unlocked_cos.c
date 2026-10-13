#include "types.h"
#include "game/campaign_map.h"

/* The COs Campaign_UnlockTopCos unlocked at the end of the campaign. */
CampaignTopCo* Campaign_GetUnlockedCos(void) { return gCampaignUnlockedCos; }
