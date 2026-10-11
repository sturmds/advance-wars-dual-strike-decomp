#include "types.h"
#include "game/campaign_recap.h"

/* TRUE past entry 27 and at the first entry whose unk_04 is 0. */
BOOL CampaignRecap_IsEnd(CampaignRecap *work, int index)
{
    if (index >= 28) {
        return TRUE;
    }
    return work->unk_7c[index].unk_04 == 0;
}
